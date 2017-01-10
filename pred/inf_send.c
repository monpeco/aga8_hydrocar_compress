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

EXEC SQL END DECLARE SECTION;



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
                and fecha  = trunc(to_date('10/01/2017', 'dd/mm/yyyy'))
            union
            select 1
            from dual
                where to_char(trunc(to_date('10/01/2017', 'dd/mm/yyyy')), 'DY', 'NLS_DATE_LANGUAGE=SPANISH') IN ('DOM', 'SÁB'));
    do_error("no_laborable()");
    
}


/* ------------------------------------------------------------------------------ */
/*                            PROCESAMIENTO DE DATOS                              */
/* ------------------------------------------------------------------------------ */

int bfnProcesar(){
    
    printf("bfnProcesar\n");


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
    if (strcpy(C002_laborable,"1") == 0 ){
        printf("Se evalua como no laborable\n");
    }else{
        printf("Laborable\n");

    }
        

    if(!bfnProcesar()){
        printf("Error\n");
        exit(1);
    }else{
        printf("OK\n");
        exit(0);
    }
}


