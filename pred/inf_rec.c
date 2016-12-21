/* ------------------------------------------------------------------------------ */
/* PROGRAMA          : odisins_mater.pc                                           */
/* DESCRIPCION       : Automatización de informe de recaudadores                  */
/* PROGRAMADOR       : AM                                                         */
/* FECHA             : Diciembre 2016                                             */
/* ------------------------------------------------------------------------------ */

EXEC SQL INCLUDE parametros.h;

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
