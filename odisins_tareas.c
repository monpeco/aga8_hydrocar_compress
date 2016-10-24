/* -----------------------------------------------------------------------------  */
/* PROGRAMA		    : odisins_tareas.pc                                           */
/* DESCRIPCION		: Órdenes de Normalización con tareas de medidor y empalme    */
/* AUTOR		    : AM                                                          */
/* FECHA		    : Octubre 2016.                                            */
/* ------------------------------------------------------------------------------ */

#include <SYNString.h>
#include <api_correo.h>			

/* Header principal (comunicación db) */
EXEC SQL INCLUDE parametros.h;	

/* Variables Generales */
#define DEBUG 0
char C255_msg[256];
char delimiter='\0';/*0x9;tab*/
int flag_records=FALSE;

/* Parametros de entrada */
char C001_par_conexion[2];
char C003_par_empresa[4];
char C007_par_rol[8];
char C010_par_tipo_tarea[11];
char C010_par_fec_inicio[11];
char C010_par_fec_fin[11];

/* Constantes interacción db */
#define NODATAFOUND 1403
#define SQLNOTFOUND ( sqlca.sqlcode == NODATAFOUND )

EXEC SQL BEGIN DECLARE SECTION;
/* Declaracion estructuras datos medidores */
char C015_nro_ord_norm[16] 		; 		EXEC SQL VAR C015_nro_ord_norm 		IS STRING(16) 	;
char C004_tipo_orden[5] 		; 		EXEC SQL VAR C004_tipo_orden 		IS STRING(5) 	;
char C010_fec_ejecucion[11] 	; 		EXEC SQL VAR C010_fec_ejecucion 	IS STRING(11) 	;
char C010_nro_suministro[11] 	; 		EXEC SQL VAR C010_nro_suministro 	IS STRING(11) 	;
char C010_tarifa[11] 			; 		EXEC SQL VAR C010_tarifa 			IS STRING(11) 	;
char C100_clave_tarifa[101] 	; 		EXEC SQL VAR C100_clave_tarifa 		IS STRING(101) 	;
char C130_tipo_ejecucion[131] 	; 		EXEC SQL VAR C130_tipo_ejecucion 	IS STRING(131) 	;
char C004_marca_equipo[5] 		; 		EXEC SQL VAR C004_marca_equipo 		IS STRING(5) 	;
char C006_cod_modelo[7] 		; 		EXEC SQL VAR C006_cod_modelo 		IS STRING(7) 	;
char C015_nro_equipo[16] 		; 		EXEC SQL VAR C015_nro_equipo 		IS STRING(16) 	;
char C100_prop_medidor[101] 	; 		EXEC SQL VAR C100_prop_medidor 		IS STRING(101) 	;
char C010_fec_hora_ini[11] 		; 		EXEC SQL VAR C010_fec_hora_ini 		IS STRING(11) 	;
char C020_fec_datos[21] 		; 		EXEC SQL VAR C020_fec_datos 		IS STRING(21) 	;

/* Declaracion estructuras datos lecturas */
char C015_marca_aparato_2[16] 		; 		EXEC SQL VAR C015_marca_aparato_2 	IS STRING(16) 	;
char C006_cod_modelo_2[7] 			; 		EXEC SQL VAR C006_cod_modelo_2 		IS STRING(7) 	;
char C015_nro_aparato_2[16] 		; 		EXEC SQL VAR C015_nro_aparato_2		IS STRING(16) 	;
char C010_fec_evento_2[11] 			; 		EXEC SQL VAR C010_fec_evento_2 		IS STRING(11) 	;
char C004_tip_medida_2[5] 			; 		EXEC SQL VAR C004_tip_medida_2 		IS STRING(5) 	;
char C016_lectura_2[17] 			; 		EXEC SQL VAR C016_lectura_2			IS STRING(17) 	;
char C013_consumo_2[14] 			; 		EXEC SQL VAR C013_consumo_2			IS STRING(14) 	;


/* Variables Uso Email */
char C1024_from_name[1024]					; EXEC SQL VAR C1024_from_name			IS STRING(1024);
char c1024_from_email[1024]					; EXEC SQL VAR c1024_from_email			IS STRING(1024);
char C1024_to_name[1024]					; EXEC SQL VAR C1024_to_name			IS STRING(1024);
char C1024_to_email[1024]					; EXEC SQL VAR C1024_to_email			IS STRING(1024);
char C1024_cc_email[1024]					; EXEC SQL VAR C1024_cc_email			IS STRING(1024);
char C1024_cco_email[1024]					; EXEC SQL VAR C1024_cco_email			IS STRING(1024);
char C1024_subject[1024]					; EXEC SQL VAR C1024_subject			IS STRING(1024);
char C2048_body[2048]						; EXEC SQL VAR C2048_body				IS STRING(2048);
char C1024_email[1024]						; EXEC SQL VAR C1024_email				IS STRING(1024);
char C050_param_adj[51]						; EXEC SQL VAR C050_param_adj			IS STRING(51);


/* Variables generales CHAR */
char	C255_nom_file[256]		;	 		EXEC SQL VAR C255_nom_file		IS STRING(256)	;

EXEC SQL END DECLARE SECTION;

/* Variables Uso General */
int flag_email=FALSE;



/*************************************************************************/
/* Concatena buffer con variables incluidas                              */
/*************************************************************************/
long strpcat(char *str, char *msg, ...)
{
    /*double *args;*/
    va_list args;
    char buffer[1024];
    va_start(args, msg);
    vsprintf(buffer,msg,args);
    va_end(args);
    strcat(str,buffer);
    return(strlen(str));
}

/*                        ABRE CURSOR PRINCIPAL                                */
SQL_OPEN_medidores(){
    char C3000_sql_sente[3001];
	
	memset(C3000_sql_sente, '\0', sizeof(C3000_sql_sente));
	
    strcpy(C3000_sql_sente, " select distinct ");
    strpcat(C3000_sql_sente,"        h6.nro_ord_norm, ");
    strpcat(C3000_sql_sente,"        decode(h6.tip_ord_norm,'I','INSP','NORM') TipoOrden, ");
    strpcat(C3000_sql_sente,"        to_char(h6.fec_hora_ini_eje,'dd/mm/yyyy') FecEjecucion, ");
    strpcat(C3000_sql_sente,"        h6.nro_suministro, ");
    strpcat(C3000_sql_sente,"        n5.campo19 Tarifa, ");
    strpcat(C3000_sql_sente,"                (select n11.descripcion  ");
    strpcat(C3000_sql_sente,"                from nucssb0011 n11  ");
    strpcat(C3000_sql_sente,"                where n11.cod_empresa = n5.cod_empresa  ");
    strpcat(C3000_sql_sente,"                and n11.nomtabla = 'CLATAR'  ");
    strpcat(C3000_sql_sente,"                and trim(n5.campo20) = n11.codigo  ");
    strpcat(C3000_sql_sente,"                ) ClaveTarifa,  ");
    strpcat(C3000_sql_sente,"                (SELECT N11.VALOR1 || ' - ' || N11.DESCRIPCION  ");
    strpcat(C3000_sql_sente,"                FROM HURSSB0039 H39,NUCSSB0011 N11  ");
    strpcat(C3000_sql_sente,"                WHERE H39.COD_EMPRESA = H6.COD_EMPRESA  ");
    strpcat(C3000_sql_sente,"                AND H39.ID_FOLIO_PRESELECC = H6.ID_FOLIO_PRESELECC  ");
    strpcat(C3000_sql_sente,"                AND N11.COD_EMPRESA = H6.COD_EMPRESA  ");
    strpcat(C3000_sql_sente,"                AND N11.NOMTABLA = 'EJEINSP'  ");
    strpcat(C3000_sql_sente,"                AND N11.CODIGO=H39.TIP_EJECUCION) TipoEjec,  ");
    strpcat(C3000_sql_sente,"        h9.marca_equipo,  ");
    strpcat(C3000_sql_sente,"        h9.cod_modelo,  ");
    strpcat(C3000_sql_sente,"        h9.nro_equipo,  ");
    strpcat(C3000_sql_sente,"                (select n11.descripcion  ");
    strpcat(C3000_sql_sente,"                from nucssb0011 n11  ");
    strpcat(C3000_sql_sente,"                where n11.cod_empresa = m11.cod_empresa  ");
    strpcat(C3000_sql_sente,"                and n11.nomtabla = 'PROMEDI'  ");
    strpcat(C3000_sql_sente,"                and m11.propiedad_equipo = n11.codigo) PropMedidor,  ");
    strpcat(C3000_sql_sente,"        h6.fec_hora_ini_eje,  ");
    strpcat(C3000_sql_sente,"        to_char(h6.fec_ingreso_datos ,'dd/mm/yyyy hh24:mi:ss') FecDatos  ");
    strpcat(C3000_sql_sente,"        from hurssb0006 h6, nucssb0005 n5, hurssb0009 h9,medssb0011 m11  ");
    strpcat(C3000_sql_sente,"        where h6.cod_empresa = %s  " , C003_par_empresa);
    strpcat(C3000_sql_sente,"        and h6.fec_hora_ini_eje >= to_date('%s' || ' 00:00:00' , 'dd/mm/yyyy hh24:mi:ss')  ", C010_par_fec_inicio);
    strpcat(C3000_sql_sente,"        and h6.fec_hora_ini_eje <= to_date('%s' || ' 23:59:59' , 'dd/mm/yyyy hh24:mi:ss')  ", C010_par_fec_fin);
    strpcat(C3000_sql_sente,"        and h6.est_orden = 'F'  ");
    strpcat(C3000_sql_sente,"        and h6.nro_suministro = n5.nro_suministro  ");
    strpcat(C3000_sql_sente,"        and h6.cod_empresa = n5.cod_empresa  ");
    strpcat(C3000_sql_sente,"        and h6.nro_ord_norm = h9.nro_ord_norm  ");
    strpcat(C3000_sql_sente,"        and h6.tip_ord_norm = h9.tip_ord_norm  ");
    strpcat(C3000_sql_sente,"        and h6.cod_empresa = h9.cod_empresa  ");
    strpcat(C3000_sql_sente,"        and h9.est_aparato = 'I'  ");
    strpcat(C3000_sql_sente,"        and h9.nro_equipo = m11.nro_aparato  ");
    strpcat(C3000_sql_sente,"        and trim(h9.marca_equipo) = m11.marca_equipo  ");
    strpcat(C3000_sql_sente,"        and h9.cod_empresa = m11.cod_empresa  ");
    strpcat(C3000_sql_sente,"        and h6.nro_suministro = m11.nro_suministro  ");
    strpcat(C3000_sql_sente,"        order by h6.fec_hora_ini_eje \n ");
	
	
	EXEC SQL DECLARE p_medidores STATEMENT;
    do_error("DECLARE p_medidores (SQL_OPEN_medidores)");

	EXEC SQL PREPARE p_medidores FROM :C3000_sql_sente;
    do_error("PREPARE p_medidores (SQL_OPEN_medidores)");

	EXEC SQL DECLARE cur_medidores CURSOR FOR p_medidores;
    do_error("DECLARE cur_medidores (SQL_OPEN_medidores)");

	EXEC SQL OPEN cur_medidores;
    do_error("OPEN cur_medidores (SQL_OPEN_medidores)");

	/* Debugger */
	if(DEBUG){
		printf("------------------------------------------------------\n");
		printf("DEBUG[SQL_OPEN_medidores]\n");
		printf("Resultado OK\n");
		printf("------------------------------------------------------\n\n");
	}	
	
	return ( TRUE );
}

/*                        FECTCH CURSOR PRINCIPAL                            */
int SQL_FETCH_medidores(){
	int		iRet;

	memset(C015_nro_ord_norm, 	'\0', sizeof(C015_nro_ord_norm));
	memset(C004_tipo_orden, '\0', sizeof(C004_tipo_orden));
	memset(C010_fec_ejecucion, '\0', sizeof(C010_fec_ejecucion));
	memset(C010_nro_suministro, '\0', sizeof(C010_nro_suministro));
	memset(C010_tarifa, '\0', sizeof(C010_tarifa));
	memset(C100_clave_tarifa, '\0', sizeof(C100_clave_tarifa));
	memset(C130_tipo_ejecucion, 	'\0', sizeof(C130_tipo_ejecucion));
	memset(C004_marca_equipo, '\0', sizeof(C004_marca_equipo));
	memset(C006_cod_modelo, '\0', sizeof(C006_cod_modelo));
	memset(C015_nro_equipo, '\0', sizeof(C015_nro_equipo));
	memset(C100_prop_medidor, '\0', sizeof(C100_prop_medidor));
	memset(C010_fec_hora_ini, '\0', sizeof(C010_fec_hora_ini));
	memset(C020_fec_datos, '\0', sizeof(C020_fec_datos));

    EXEC SQL 
		FETCH cur_medidores 
		INTO :C015_nro_ord_norm,:C004_tipo_orden,:C010_fec_ejecucion,:C010_nro_suministro,:C010_tarifa,
		:C100_clave_tarifa,:C130_tipo_ejecucion,:C004_marca_equipo,:C006_cod_modelo,:C015_nro_equipo,
		:C100_prop_medidor,:C010_fec_hora_ini,:C020_fec_datos;

    iRet = do_error("FETCH cur_medidores (SQL_FETCH_medidores)");
    if ( iRet == TRUE )
        return ( FALSE );

	/* Debugger */
	if(DEBUG){
		printf("------------------------------------------------------\n");
		printf("DEBUG[SQL_FETCH_medidores]\n");
		printf("Resultado OK\n");
		printf("------------------------------------------------------\n\n");
	}

	if(SQLNOTFOUND){return ( FALSE );}
	else		   {return ( TRUE );}
}


/* ------------------------------------------------------------------------- */
/*                        PROCESAMIENTO DE DATOS                                */
/* ------------------------------------------------------------------------- */
int bfnProcesar(){
	FILE	*fpMedidores=NULL;
	char    C2000_Buffer[2001]; 	EXEC SQL VAR C2000_Buffer IS STRING(2001) ;
	int iLecturas = 0;
	int i = 0;

	if (SQL_OPEN_medidores())
	{
		while (SQL_FETCH_medidores())
		{

		}
	
	}
		
	return ( TRUE );
}

/* ------------------------------------------------------------------------- */
/*                        PROGRAMA  PRINCIPAL                                */
/* ------------------------------------------------------------------------- */
main(int argc,char **argv)
{
   if( argc != 6)
    {
	printf("Use : %s <N° Conexion> <Empresa> <Rol> <Tipo de Tarea> <FecInicio> <FecFin> \n",argv[0]);
	exit(1);
    }

	strcpy( C001_par_conexion,			argv[1] );
	strcpy( C003_par_empresa,			argv[2] );
	strcpy( C007_par_rol,				argv[3] );
	strcpy( C010_par_tipo_tarea,		argv[4] );
	strcpy( C010_par_fec_inicio,		argv[5] );
	strcpy( C010_par_fec_fin,			argv[6] );

	sql_conexion(C001_par_conexion);

	
	if(!bfnProcesar()){
		printf("Error\n");
		exit(1);
	}else{
		printf("OK\n");
		exit(0);
	}
}               
