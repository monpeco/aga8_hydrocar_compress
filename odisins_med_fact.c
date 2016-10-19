/* -----------------------------------------------------------------------------  */
/* PROGRAMA		    : odisins_med_fact.pc                                         */
/* DESCRIPCION		:                                                             */
/* AUTOR		    :                                                             */
/* FECHA		    : Octubre 2016.                                               */
/* ------------------------------------------------------------------------------ */

#include <SYNString.h>
#include <api_correo.h>			

/* Header principal (comunicación db) */
EXEC SQL INCLUDE parametros.h;	

/* Variables Generales */
#define DEBUG 0
char C255_msg[256];

/* Parametros de entrada */
char C001_par_conexion[2];
char C003_par_empresa[4];
char C007_par_rol[8];
char C010_par_fec_inicio[11];
char C010_par_fec_fin[11];

/* Constantes interacción db */
#define NODATAFOUND 1403
#define SQLNOTFOUND ( sqlca.sqlcode == NODATAFOUND )
#define NOTLOGGEDON 1012
#define SQLNOTLOGON ( sqlca.sqlcode == NOTLOGGEDON )
#define SQLOKDATA	( sqlca.sqlerrd[2] == 1 )

/* Variables interactuan con archivo plano */
#define	TAM_REGISTRO		  100	
#define	CANT_REG_ARCHIVO	20000

EXEC SQL BEGIN DECLARE SECTION;


EXEC SQL END DECLARE SECTION;

/* Variables Uso General */
int iCountLeidos=0;
int iCountUpd=0;
int iCountRechz=0;



int bfnProcesar(){
	
	return ( TRUE );
	
}
/* ------------------------------------------------------------------------- */
/*                        PROGRAMA  PRINCIPAL                                */
/* ------------------------------------------------------------------------- */
main(int argc,char **argv)
{
   if( argc != 6)
    {
	printf("Use : %s <N° Conexion> <Empresa> <Rol> <FecInicio> <FecFin> \n",argv[0]);
	exit(1);
    }

	strcpy( C001_par_conexion,		argv[1] );
	strcpy( C003_par_empresa,		argv[2] );
	strcpy( C007_par_rol,			argv[3] );
    strcpy( C010_par_fec_inicio,	argv[4] );
    strcpy( C010_par_fec_fin,		argv[5] );
	

	
	if(!bfnProcesar()){
		printf("Error\n");
		exit(1);
	}else{
		printf("OK\n");
		exit(0);
	}
}               
