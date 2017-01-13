/* ------------------------------------------------------------------------------ */
/* PROGRAMA          : inf_send.pc                                                */
/* DESCRIPCION       : Envio de informe de recaudadores                           */
/* PROGRAMADOR       : AM                                                         */
/* FECHA             : Enero 2017                                                 */
/* ------------------------------------------------------------------------------ */
#include <api_correo.h>			

EXEC SQL INCLUDE parametros.h;

const char *PATH = "/tmp/";
const char SEPARADOR = ';';
/* ------------------------------------------------------------------------------ */
/*                            DECLARACIÓN DE VARIABLES                            */
/* ------------------------------------------------------------------------------ */
char C001_par_conexion[2];
char C255_nom_file[256];

/* Variables Generales */
#define DEBUG 0

/* Constantes interacción db */
#define NODATAFOUND 1403
#define SQLNOTFOUND ( sqlca.sqlcode == NODATAFOUND )

/* Variables Email */
char C1024_from_name[1024];
char c1024_from_email[1024];
char C1024_to_name[1024];
char C1024_to_email[1024];
char C1024_cc_email[1024];
char C1024_subject[1024];
char C2048_body[2048];

EXEC SQL BEGIN DECLARE SECTION;


char	C002_laborable[2];	EXEC SQL VAR C002_laborable IS STRING(2);

char C003_dia_semana[15]      ;    EXEC SQL VAR C003_dia_semana IS STRING(15)      ;
char C003_dia_mes[3]          ;    EXEC SQL VAR C003_dia_mes IS STRING(3)          ;
char C003_mes[15]             ;    EXEC SQL VAR C003_mes IS STRING(15)             ;
char C003_ano[5]              ;    EXEC SQL VAR C003_ano IS STRING(5)              ;

EXEC SQL END DECLARE SECTION;


void crear_lista(){
    char C256_pat_unix[256]; //EXEC SQL VAR C256_pat_unix IS STRING(256);
    
    memset(C256_pat_unix, '\0', sizeof(C256_pat_unix));

    /* Obtiene path unix */
    strcpy(C256_pat_unix, "ls ");
    strcat(C256_pat_unix, PATH);
    strcat(C256_pat_unix, "*.csv.gz > ");
    strcat(C256_pat_unix, PATH);
    strcat(C256_pat_unix, "lista_inf.txt");
    
    system(C256_pat_unix);
}

void enviar_correo(){
    char C256_pat_unix[256];
    FILE   *fpe;
    int     contador=0;
    char    xstring[200];
    char    file_date_day[20];
    char    file_date_month[20];
    char    file_date_year[20];
    
    /* Variables Email */
    char C1024_from_name[1024];
    char c1024_from_email[1024];
    char C1024_to_name[1024];
    char C1024_to_email[1024];
    char C1024_cc_email[1024];
    char C1024_subject[1024];
    char C2048_body[2048];
    
    char C256_rm[256];
    
    memset(C256_pat_unix, '\0', sizeof(C256_pat_unix));
    memset(xstring, '\0', sizeof(xstring));
    
    memset(C1024_from_name, '\0', sizeof(C1024_from_name));
    memset(c1024_from_email, '\0', sizeof(c1024_from_email));
    memset(C1024_subject, '\0', sizeof(C1024_subject));
    memset(C2048_body, '\0', sizeof(C2048_body));
    memset(C1024_to_name, '\0', sizeof(C1024_to_name));
    memset(C1024_to_email, '\0', sizeof(C1024_to_email));
    memset(C1024_cc_email, '\0', sizeof(C1024_cc_email));

    
    int iRet=0;

    /* Obtiene fecha del sistema */
    EXEC SQL
        SELECT TRIM(TO_CHAR(SYSDATE,'Day'))
            , TRIM(TO_CHAR(SYSDATE,'DD'))
            , TRIM(TO_CHAR(SYSDATE,'Month'))
            , TRIM(TO_CHAR(SYSDATE,'YYYY'))
        INTO    :C003_dia_semana, :C003_dia_mes, :C003_mes, C003_ano
        FROM DUAL;

    iRet = do_error("Select SYSDATE - DAY DD MONTH YYYY");
    if ( iRet == TRUE )
        return ( FALSE );


    strcpy(C1024_from_name, "noreply");
    strcpy(c1024_from_email, "ramm@tivit-synapsis.com");
    strcpy(C1024_subject, "Informe Recaudadores");
    strcpy(C1024_to_email, "AM@tivit-synapsis.com, monpeco@gmail.com");

        
    strcpy(C256_pat_unix, PATH);
    strcat(C256_pat_unix, "lista_inf.txt");
    fpe = fopen(C256_pat_unix, "r");
    while ( fgets(xstring,100,fpe) != NULL )
    {
        imp_trim(xstring);
        memset(C2048_body, '\0', sizeof(C2048_body));
        
        memset(file_date_day, '\0', sizeof(file_date_day));
        memset(file_date_month, '\0', sizeof(file_date_month));
        memset(file_date_year, '\0', sizeof(file_date_year));
        
        memcpy( file_date_day, &xstring[26], 2 );
        file_date_day[2] = '\0';
        
        memcpy( file_date_month, &xstring[28], 2 );
        file_date_month[2] = '\0';

        memcpy( file_date_year, &xstring[30], 4 );
        file_date_year[4] = '\0';

        strpcat(C2048_body, "Estimados,\n\nAdjunto archivo correspondiente al día %s/%s/%s.\n\nAtte.\nRodrigo Mugoreni", file_date_day, file_date_month, file_date_year);
      
        correo_head(C1024_from_name, c1024_from_email, C1024_to_name, C1024_to_email, C1024_cc_email, C1024_subject);
        correo_body(C2048_body);
        
        xstring[ strlen(xstring)-1 ] = '\0';
        correo_attach(xstring, "");
        
        if (!correo_enviar()){
            printf(correo_error);
            return ( FALSE );
        }
        
        memset(C256_rm, '\0', sizeof(C256_rm));
        strpcat(C256_rm, "rm %s", xstring);
        system(C256_rm);

    }    

}

void no_laborable(){
    
    memset(C002_laborable, '\0', sizeof(C002_laborable));
    

    EXEC SQL
        select nvl(max(xxx),0)
        INTO :C002_laborable
        from ( select 1 xxx
            from nucssb0054
                where cod_empresa in (1,2)
                and est_registro = 'A'
                and cod_sistema = 'AIC'
            and fecha  = sysdate-1
        union
        select 1
        from dual
            where to_char(sysdate-1, 'DY', 'NLS_DATE_LANGUAGE=SPANISH') IN ('DOM', 'SÁB'));
    do_error("no_laborable()");

    
}


/* ------------------------------------------------------------------------------ */
/*                            PROCESAMIENTO DE DATOS                              */
/* ------------------------------------------------------------------------------ */

int bfnProcesar(){
    printf("bfnProcesar\n");

    crear_lista();
    enviar_correo();

    return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            PROGRAMA  PRINCIPAL                                 */
/* ------------------------------------------------------------------------------ */
main(int argc,char **argv)
{
   if( argc != 2){
        printf("Use : %s <conexion>\n",argv[0]);
        exit(1);
    }

    strcpy(C001_par_conexion, argv[1]);

    sql_conexion(C001_par_conexion);
    no_laborable();
    if (strcmp(C002_laborable, "1") == 0 ){
        printf("Se evalua como no laborable\n");
        exit(0);
    }

    if(!bfnProcesar()){
        printf("Error\n");
        exit(1);
    }else{
        printf("OK\n");
        exit(0);
    }
}

