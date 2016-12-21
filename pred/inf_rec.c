/* ------------------------------------------------------------------------------ */
/* PROGRAMA          : odisins_mater.pc                                           */
/* DESCRIPCION       : Automatización de informe de recaudadores                  */
/* PROGRAMADOR       : AM                                                         */
/* FECHA             : Diciembre 2016                                             */
/* ------------------------------------------------------------------------------ */

EXEC SQL INCLUDE parametros.h;

/* ------------------------------------------------------------------------------ */
/*                            DECLARACIÓN DE VARIABLES                            */
/* ------------------------------------------------------------------------------ */
char C010_par_fec_proceso[11];

/* Variables Generales */
#define DEBUG 0

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

EXEC SQL END DECLARE SECTION;

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
    strpcat(C4000_sql_sente,   "    and cod_cajero in (97, -- caja vecina 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       98, -- serviestado 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       99, -- portal intenet 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       95, -- servipag 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       96, -- servipag expres 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       88, -- presto 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       98, -- unired 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       89, -- serviestado 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       87  -- web pay 14:00 a 13:59   "  );
    strpcat(C4000_sql_sente,   "                      )  "  );
    strpcat(C4000_sql_sente,   "    and FEC_PAGO between to_date('%s'||' 14:00:00','dd/mm/yyyy hh24:mi:ss') and to_date('%s'||' 13:59:59','dd/mm/yyyy hh24:mi:ss')+1  "  , C010_par_fec_proceso, C010_par_fec_proceso);
    strpcat(C4000_sql_sente,   "  union  "  );
    strpcat(C4000_sql_sente,   "  select COD_EMPRESA, COD_OFICINA, COD_CAJERO, EST_PAGO, FEC_PAGO, FEC_REGISTRO, NRO_SUMINISTRO, NRO_TRANSACCION, NRO_TRAN_RECA, MTO_PAGO, TIP_PAGO,  "  );
    strpcat(C4000_sql_sente,   "         decode(cod_cajero,4015,1,4016,2,4017,3,4022,4,4013,5,4017,6,4020,7,4021,8,4019,9) orden  "  );
    strpcat(C4000_sql_sente,   "  from RECSSB0034 r34  "  );
    strpcat(C4000_sql_sente,   "  where cod_empresa = 2  "  );
    strpcat(C4000_sql_sente,   "    and cod_cajero in (4015, -- caja vecina 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4016, -- serviestado 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4017, -- portal intenet 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4013, -- servipag 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4017, -- servipag expres 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4022, -- bancoestado  "  );
    strpcat(C4000_sql_sente,   "                       4020, -- presto 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4021, -- unired 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                       4019 -- web pay 14:00 a 13:59  "  );
    strpcat(C4000_sql_sente,   "                      )  "  );
    strpcat(C4000_sql_sente,   "   and FEC_PAGO between to_date('%s'||' 14:00:00','dd/mm/yyyy hh24:mi:ss') and to_date('%s'||' 13:59:59','dd/mm/yyyy hh24:mi:ss')+1  "  , C010_par_fec_proceso, C010_par_fec_proceso);
    strpcat(C4000_sql_sente,   "  order by COD_EMPRESA, orden  "  );
    strpcat(C4000_sql_sente,   "  )  "  );

    EXEC SQL DECLARE p_medidores STATEMENT;
    do_error("DECLARE p_medidores (SQL_OPEN_medidores)");

    EXEC SQL PREPARE p_medidores FROM :C4000_sql_sente;
    do_error("PREPARE p_medidores (SQL_OPEN_medidores)");

    EXEC SQL DECLARE cur_medidores CURSOR FOR p_medidores;
    do_error("DECLARE cur_medidores (SQL_OPEN_medidores)");

    EXEC SQL OPEN cur_medidores;
    do_error("OPEN cur_medidores (SQL_OPEN_medidores)");

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
/*                            PROCESAMIENTO DE DATOS                              */
/* ------------------------------------------------------------------------------ */
int bfnProcesar(){
    FILE    *fpRecaudadores=NULL;
    char    C5000_Buffer[5001];    EXEC SQL VAR C5000_Buffer IS STRING(5001);
    
    if (SQL_OPEN_recaudadores())
	{
        
    }
    return ( TRUE );
}
/* ------------------------------------------------------------------------------ */
/*                            PROGRAMA  PRINCIPAL                                 */
/* ------------------------------------------------------------------------------ */
main(int argc,char **argv)
{
   if( argc != 2){
        printf("Use : %s <fecha_proceso>\n",argv[0]);
        exit(1);
    }
    
    strcpy(C010_par_fec_proceso, argv[1]);


    if(!bfnProcesar()){
        printf("Error\n");
        exit(1);
    }else{
        printf("OK\n");
        exit(0);
    }
}               