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
| MTO_PAGO	       | NUMBER (15)        | C015_mto_pago[16]        | 
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
/*                            PROCESAMIENTO DE DATOS                              */
/* ------------------------------------------------------------------------------ */

int bfnProcesar(){
    
    
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



    if(!bfnProcesar()){
        printf("Error\n");
        exit(1);
    }else{
        printf("OK\n");
        exit(0);
    }
}               