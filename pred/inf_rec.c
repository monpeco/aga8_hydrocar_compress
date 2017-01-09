/* ------------------------------------------------------------------------------ */
/* PROGRAMA          : inf_rec.pc                                                 */
/* DESCRIPCION       : Automatización de informe de recaudadores                  */
/* PROGRAMADOR       : AM                                                         */
/* FECHA             : Diciembre 2016                                             */
/* ------------------------------------------------------------------------------ */
#include <api_correo.h>			

EXEC SQL INCLUDE parametros.h;

const char *PATH = "/tmp/";
const char SEPARADOR = ';';
/* ------------------------------------------------------------------------------ */
/*                            DECLARACIÓN DE VARIABLES                            */
/* ------------------------------------------------------------------------------ */
char C010_par_fec_proceso[11];
char C010_par_fec_proceso_numeros[11];
char C001_par_conexion[2];
//char	C255_nom_file[256]		;	 		EXEC SQL VAR C255_nom_file		IS STRING(256)	;
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
/* Declaracion estructuras recaudadores 
| Query field      | Date type          | variables                |
|------------------|--------------------|--------------------------|
| COD_EMPRESA      | NUMBER (2)         | C002_cod_empresa[3]      | 
| COD_OFICINA      | VARCHAR2 (4 Byte)  | C004_cod_oficina[5]      | 
| COD_CAJERO       | VARCHAR2 (4 Byte)  | C004_cod_cajero[5]       | 
| EST_PAGO         | VARCHAR2 (2 Byte)  | C002_est_pago[3]         | 
| FEC_PAGO         | DATE               | C010_fec_pago[11]        | 
| FEC_REGISTRO     | DATE               | C010_fec_registro[11]    | 
| NRO_SUMINISTRO   | NUMBER (10)        | C010_nro_suministro[11]  | 
| NRO_TRANSACCION  | NUMBER             | C022_nro_transaccion[23] | 
| NRO_TRAN_RECA    | VARCHAR2 (50 Byte) | C050_nro_tran_reca[51]   | 
| MTO_PAGO         | NUMBER (15)        | C015_mto_pago[16]        | 
| TIP_PAGO         | VARCHAR2 (3 Byte)  | C003_tip_pago[4]         | 
*/

char C002_cod_empresa[3]      ;    EXEC SQL VAR C002_cod_empresa IS STRING(3)      ;
char C004_cod_oficina[5]      ;    EXEC SQL VAR C004_cod_oficina IS STRING(5)      ;
char C004_cod_cajero[5]       ;    EXEC SQL VAR C004_cod_cajero IS STRING(5)       ;
char C002_est_pago[3]         ;    EXEC SQL VAR C002_est_pago IS STRING(3)         ;
char C010_fec_pago[11]        ;    EXEC SQL VAR C010_fec_pago IS STRING(11)        ;
char C010_fec_registro[11]    ;    EXEC SQL VAR C010_fec_registro IS STRING(11)    ;
char C010_nro_suministro[11]  ;    EXEC SQL VAR C010_nro_suministro IS STRING(11)  ;
char C022_nro_transaccion[23] ;    EXEC SQL VAR C022_nro_transaccion IS STRING(23) ;
char C050_nro_tran_reca[51]   ;    EXEC SQL VAR C050_nro_tran_reca IS STRING(51)   ;
char C015_mto_pago[16]        ;    EXEC SQL VAR C015_mto_pago IS STRING(16)        ;
char C003_tip_pago[4]         ;    EXEC SQL VAR C003_tip_pago IS STRING(4)         ;

char C003_dia_semana[15]      ;    EXEC SQL VAR C003_dia_semana IS STRING(15)      ;
char C003_dia_mes[3]          ;    EXEC SQL VAR C003_dia_mes IS STRING(3)          ;
char C003_mes[15]             ;    EXEC SQL VAR C003_mes IS STRING(15)             ;
char C003_ano[5]              ;    EXEC SQL VAR C003_ano IS STRING(5)              ;

EXEC SQL END DECLARE SECTION;




/* ------------------------------------------------------------------------------ */
/*                            Envia email segun parametros de configuracion       */
/* ------------------------------------------------------------------------------ */
int ifnSendEmail(){
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
    strpcat(C2048_body, "Estimados,\n\nAdjunto archivo correspondiente al día %s, %s de %s de %s.\n\nAtte.\nRodrigo Mugoreni", C003_dia_semana, C003_dia_mes, C003_mes, C003_ano);
    //strcpy(C1024_to_name, "grupo_esval");
    strcpy(C1024_to_email, "AM@tivit-synapsis.com, monpeco@gmail.com");
    //strcpy(C1024_cc_email, "monpeco@gmail.com");

    correo_head(C1024_from_name, c1024_from_email, C1024_to_name, C1024_to_email, C1024_cc_email, C1024_subject);
    correo_body(C2048_body);
    strcat(C255_nom_file, ".gz");
    correo_attach(C255_nom_file, "");
    
    if (!correo_enviar()){
        printf(correo_error);
        return ( FALSE );
    }

    if(DEBUG){
    printf("------------------------------------------------------\n");
    printf("DEBUG[ifnSendEmail]\n");
    printf("Resultado OK\n");
    printf("------------------------------------------------------\n\n");
    }

    return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            Crea archivo de salida en base a los prefijos       */
/* ------------------------------------------------------------------------------ */
int bfnCrearArchivoSalida(FILE **fpOut, char *prefix1, char *prefix2, char *ext){
    int iRet=0;
    char C256_pat_unix[256]; EXEC SQL VAR C256_pat_unix IS STRING(256);
    char C020_fecha[20]; EXEC SQL VAR C020_fecha IS STRING(20);

    memset(C256_pat_unix, '\0', sizeof(C256_pat_unix));
    memset(C020_fecha, '\0', sizeof(C020_fecha));
    memset(C255_nom_file, '\0', sizeof(C255_nom_file));

    /* Obtiene path unix */
    strcpy(C256_pat_unix, PATH);

	/* Obtiene fecha del sistema */
    EXEC SQL
         SELECT  TO_CHAR( sysdate, 'ddmmyyyy' )
         INTO    :C020_fecha
         FROM    DUAL;
    iRet = do_error("Select SYSDATE");
    if ( iRet == TRUE )
        return ( FALSE );

    C010_par_fec_proceso_numeros[2] = C010_par_fec_proceso_numeros[3];
    C010_par_fec_proceso_numeros[3] = C010_par_fec_proceso_numeros[4];
    C010_par_fec_proceso_numeros[4] = C010_par_fec_proceso_numeros[6];
    C010_par_fec_proceso_numeros[5] = C010_par_fec_proceso_numeros[7];
    C010_par_fec_proceso_numeros[6] = C010_par_fec_proceso_numeros[8];
    C010_par_fec_proceso_numeros[7] = C010_par_fec_proceso_numeros[9];
    C010_par_fec_proceso_numeros[8] = '\0';
    
	/* Crea archivo de salida */
	sprintf(C255_nom_file, "%s%s_%s_%s.%s", C256_pat_unix,prefix1,prefix2,C010_par_fec_proceso_numeros,ext);
	if( ( *fpOut = fopen( C255_nom_file, "w+" ) ) == NULL )
	{
		printf("ERR|Error al Generar Archivo <%s>. Error %s\n", 
			   C255_nom_file, strerror(errno));
		return ( FALSE );
	}

	/* Debugger */
	if(DEBUG){
		printf("------------------------------------------------------\n");
		printf("DEBUG[bfnCrearArchivoSalida]\n");
		printf("Resultado [%s] OK\n",C255_nom_file);
		printf("------------------------------------------------------\n\n");
	}

	return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            Agrega data al archivo de salida                    */
/* ------------------------------------------------------------------------------ */
int bfnAgregarArchivoSalida(FILE *fpOut, char *cBuffer){/**/
	int iRet = TRUE;

	fprintf(fpOut, "%s",cBuffer);

	/* Debugger */
	if(DEBUG){
		printf("------------------------------------------------------\n");
		printf("DEBUG[bfnAgregarArchivoSalida]\n");
		printf("[%s\n]",cBuffer);
		printf("Resultado OK\n");
		printf("------------------------------------------------------\n\n");
	}

	return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            Cierra archivo de salida                            */
/* ------------------------------------------------------------------------------ */
int bfnCerrarArchivoSalida(FILE **fpOut){/**/
	fclose ( *fpOut );

	return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            Comprimir archivo de salida                         */
/* ------------------------------------------------------------------------------ */
int bfnComprimirArchivoSalida(){
    char command[300];
    memset(command, '\0', sizeof(command));

    sprintf(command, "gzip -9 %s", C255_nom_file);
    system(command);

}
/* ------------------------------------------------------------------------------ */
/*                            ABRE CURSOR PRINCIPAL                               */
/* ------------------------------------------------------------------------------ */
SQL_OPEN_recaudadores(){
    char C4000_sql_sente[4001];
    memset(C4000_sql_sente, '\0', sizeof(C4000_sql_sente));
    
    strcpy(C4000_sql_sente, "  ");
    strpcat(C4000_sql_sente,   "  select COD_EMPRESA, COD_OFICINA, COD_CAJERO, EST_PAGO, FEC_PAGO, FEC_REGISTRO, NRO_SUMINISTRO, NRO_TRANSACCION, NRO_TRAN_RECA, MTO_PAGO, TIP_PAGO  "  );
    strpcat(C4000_sql_sente,   "  from (  "  );
    strpcat(C4000_sql_sente,   "  select COD_EMPRESA, COD_OFICINA, COD_CAJERO, EST_PAGO, FEC_PAGO, FEC_REGISTRO, NRO_SUMINISTRO, NRO_TRANSACCION, NRO_TRAN_RECA, MTO_PAGO, TIP_PAGO,  "  );
    strpcat(C4000_sql_sente,   "         decode(cod_cajero,0097,1,0098,2,0089,3,0099,4,0095,5,0096,6,0088,7,0098,8,0087,9) orden  "  );
    strpcat(C4000_sql_sente,   "  from RECSSB0034 r34  "  );
    strpcat(C4000_sql_sente,   "  where cod_empresa = 1  "  );
    strpcat(C4000_sql_sente,   "    and cod_cajero in (97, /* caja vecina 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       98, /* serviestado 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       99, /* portal intenet 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       95, /* servipag 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       96, /* servipag expres 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       88, /* presto 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       98, /* unired 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       89, /* serviestado 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       87  /* web pay 14:00 a 13:59 */  "  );
    strpcat(C4000_sql_sente,   "                      )  "  );
    strpcat(C4000_sql_sente,   "    and fec_registro between to_date('%s'||' 14:00:01','dd/mm/yyyy hh24:mi:ss') and to_date('%s'||' 14:00:00','dd/mm/yyyy hh24:mi:ss')+1  " , C010_par_fec_proceso, C010_par_fec_proceso );
    strpcat(C4000_sql_sente,   "  union  "  );
    strpcat(C4000_sql_sente,   "  select COD_EMPRESA, COD_OFICINA, COD_CAJERO, EST_PAGO, FEC_PAGO, FEC_REGISTRO, NRO_SUMINISTRO, NRO_TRANSACCION, NRO_TRAN_RECA, MTO_PAGO, TIP_PAGO,  "  );
    strpcat(C4000_sql_sente,   "         decode(cod_cajero,4015,1,4016,2,4017,3,4022,4,4013,5,4017,6,4020,7,4021,8,4019,9) orden  "  );
    strpcat(C4000_sql_sente,   "  from RECSSB0034 r34  "  );
    strpcat(C4000_sql_sente,   "  where cod_empresa = 2  "  );
    strpcat(C4000_sql_sente,   "    and cod_cajero in (4015, /* caja vecina 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4016, /* serviestado 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4017, /* portal intenet 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4013, /* servipag 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4017, /* servipag expres 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4022, /* bancoestado */ "  );
    strpcat(C4000_sql_sente,   "                       4020, /* presto 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4021, /* unired 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                       4019 /* web pay 14:00 a 13:59 */ "  );
    strpcat(C4000_sql_sente,   "                      )  "  );
    strpcat(C4000_sql_sente,   "    and fec_registro between to_date('%s'||' 14:00:01','dd/mm/yyyy hh24:mi:ss') and to_date('%s'||' 14:00:00','dd/mm/yyyy hh24:mi:ss')+1  " , C010_par_fec_proceso, C010_par_fec_proceso );
    strpcat(C4000_sql_sente,   "  order by COD_EMPRESA, orden  "  );
    strpcat(C4000_sql_sente,   "  )  "  );

    EXEC SQL DECLARE p_recaudadores STATEMENT;
    do_error("DECLARE p_recaudadores (SQL_OPEN_recaudadores)");

    EXEC SQL PREPARE p_recaudadores FROM :C4000_sql_sente;
    do_error("PREPARE p_recaudadores (SQL_OPEN_recaudadores)");

    EXEC SQL DECLARE cur_recaudadores CURSOR FOR p_recaudadores;
    do_error("DECLARE cur_recaudadores (SQL_OPEN_recaudadores)");

    EXEC SQL OPEN cur_recaudadores;
    do_error("OPEN cur_recaudadores (SQL_OPEN_recaudadores)");

    /* Debugger */
    if(DEBUG){
        printf("------------------------------------------------------\n");
        printf("DEBUG[SQL_OPEN_recaudadores]\n");
        printf("Resultado OK\n");
        printf("------------------------------------------------------\n\n");
    }	

    return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            FETCH CURSOR PRINCIPAL                              */
/* ------------------------------------------------------------------------------ */
int SQL_FETCH_recaudadores(){
    int iRet=0;
    
    memset(C002_cod_empresa,'\0', sizeof(C002_cod_empresa));
    memset(C004_cod_oficina,'\0', sizeof(C004_cod_oficina));
    memset(C004_cod_cajero,'\0', sizeof(C004_cod_cajero));
    memset(C002_est_pago,'\0', sizeof(C002_est_pago));
    memset(C010_fec_pago,'\0', sizeof(C010_fec_pago));
    memset(C010_fec_registro,'\0', sizeof(C010_fec_registro));
    memset(C010_nro_suministro,'\0', sizeof(C010_nro_suministro));
    memset(C022_nro_transaccion,'\0', sizeof(C022_nro_transaccion));
    memset(C050_nro_tran_reca,'\0', sizeof(C050_nro_tran_reca));
    memset(C015_mto_pago,'\0', sizeof(C015_mto_pago));
    memset(C003_tip_pago,'\0', sizeof(C003_tip_pago));

    EXEC SQL  
        FETCH cur_recaudadores 
        INTO :C002_cod_empresa,:C004_cod_oficina,:C004_cod_cajero,:C002_est_pago,:C010_fec_pago
        ,:C010_fec_registro,:C010_nro_suministro,:C022_nro_transaccion,:C050_nro_tran_reca,:C015_mto_pago
        ,:C003_tip_pago;

    iRet = do_error("FETCH cur_recaudadores (SQL_FETCH_recaudadores)");
    if ( iRet == TRUE )
        return ( FALSE );

    /* Debugger */
    if(DEBUG){
        printf("------------------------------------------------------\n");
        printf("DEBUG[SQL_FETCH_recaudadores]\n");
        printf("Resultado OK\n");
        printf("------------------------------------------------------\n\n");
    }

    if(SQLNOTFOUND){return ( FALSE );}
    else {return ( TRUE );}
}    
/* ------------------------------------------------------------------------------ */
/*                            PROCESAMIENTO DE DATOS                              */
/* ------------------------------------------------------------------------------ */
int bfnProcesar(){
    FILE    *fpRecaudadores=NULL;
    char    C5000_Buffer[5001];    EXEC SQL VAR C5000_Buffer IS STRING(5001);
    int flag_records=FALSE;
    char delimiter=SEPARADOR;
    
    if (SQL_OPEN_recaudadores()){
        
        while (SQL_FETCH_recaudadores()){
            
            memset(C5000_Buffer, '\0', sizeof(C5000_Buffer));
            if(!flag_records){ /*controla si hay registros*/
                flag_records=TRUE;
                if(!bfnCrearArchivoSalida(&fpRecaudadores,"Informe","Recaudadores","csv")){
                    return ( FALSE );
                }
                
                /* Encabezado archivo */
                strpcat(C5000_Buffer,"%-11.11s","COD_EMPRESA");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-11.11s","COD_OFICINA");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-10.10s","COD_CAJERO");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-8.8s","EST_PAGO");
                strpcat(C5000_Buffer,"%c",delimiter); 
                strpcat(C5000_Buffer,"%-11.11s","FEC_PAGO");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-11.11s","FEC_REGISTRO");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-11.11s","NRO_SUMINISTRO");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-23.23s","NRO_TRANSACCION");
                strpcat(C5000_Buffer,"%c",delimiter); 
                strpcat(C5000_Buffer,"%-51.51s","NRO_TRAN_RECA");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-16.16s","MTO_PAGO");
                strpcat(C5000_Buffer,"%c",delimiter);
                strpcat(C5000_Buffer,"%-8.8s","TIP_PAGO");
                strpcat(C5000_Buffer,"%c",delimiter);
                
                strpcat(C5000_Buffer,"%s","\n");
                
            }
              
        /* Archivo de lecturas */
        strpcat(C5000_Buffer,"%-11.11s",C002_cod_empresa);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-11.11s",C004_cod_oficina);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-10.10s",C004_cod_cajero);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-8.8s",C002_est_pago);
        strpcat(C5000_Buffer,"%c",delimiter); 
        strpcat(C5000_Buffer,"%-11.11s",C010_fec_pago);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-11.11s",C010_fec_registro);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-11.11s",C010_nro_suministro);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-23.23s",C022_nro_transaccion);
        strpcat(C5000_Buffer,"%c",delimiter); 
        strpcat(C5000_Buffer,"%-51.51s",C050_nro_tran_reca);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-16.16s",C015_mto_pago);
        strpcat(C5000_Buffer,"%c",delimiter);
        strpcat(C5000_Buffer,"%-4.4s",C003_tip_pago);
        strpcat(C5000_Buffer,"%c",delimiter);
        
        strpcat(C5000_Buffer,"%s","\n");

        bfnAgregarArchivoSalida(fpRecaudadores,C5000_Buffer);
        /* Fin archivo de Recaudadores */        
        
        }

        if(flag_records)
        {
            bfnCerrarArchivoSalida(&fpRecaudadores);
            bfnComprimirArchivoSalida();
            ifnSendEmail();
        }        
    }
    return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            PROGRAMA  PRINCIPAL                                 */
/* ------------------------------------------------------------------------------ */
main(int argc,char **argv)
{
   if( argc != 3){
        printf("Use : %s <conexion> <fecha_proceso>\n",argv[0]);
        exit(1);
    }

    strcpy(C001_par_conexion, argv[1]);
    strcpy(C010_par_fec_proceso, argv[2]);
    strcpy(C010_par_fec_proceso_numeros, C010_par_fec_proceso);
    
    sql_conexion(C001_par_conexion);

    if(!bfnProcesar()){
        printf("Error\n");
        exit(1);
    }else{
        printf("OK\n");
        exit(0);
    }
}

