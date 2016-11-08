/* -----------------------------------------------------------------------------	*/
/* PROGRAMA			: odisins_mater.pc												*/
/* DESCRIPCION		: 2.5.6.7	Listado de materiales por tipo de trabajo		 	*/
/* AUTOR			: AM															*/
/* FECHA			:  Noviembre 2016.											*/
/* ------------------------------------------------------------------------------	*/

#include <SYNString.h>
#include <api_correo.h>			

/* Header principal (comunicación db) */
EXEC SQL INCLUDE parametros.h;	

/* Variables Generales */
#define DEBUG 0
char delimiter='\0';/*0x9;tab*/
int flag_records=FALSE;

/* Parametros de entrada */
char C001_par_conexion[2];
char C003_par_empresa[4];
char C007_par_rol[8];
char C010_par_tipo_ejecucion[11];
char C010_par_origen[11];
char C010_par_fec_inicio[11];
char C010_par_fec_fin[11];

/* Constantes interacción db */
#define NODATAFOUND 1403
#define SQLNOTFOUND ( sqlca.sqlcode == NODATAFOUND )

EXEC SQL BEGIN DECLARE SECTION;
/* Declaracion estructuras datos medidores */
/*
variables					columna_archivo			query				tipo_dato_tabla
C015_nro_ord_norm[16]		Nro. NORM 				NROORDEN			NUMBER (15)
C010_nro_suministro[11]		Nro. Cliente			CLIENTE				NUMBER (10)
C010_tarifa[11]				Tarifa					TARIFA				CHAR (10 Byte)
C100_clave_tarifa[101]		Clave tarifa			CLAVETARIFA			VARCHAR2 (100 Byte)
C100_comuna[101]			Comuna					COMUNA				VARCHAR2 (100 Byte)
C003_capacidad[4]			Capacidad empalme		CAPACIDAD			3
C020_fecha_ejecucion[21]	Fecha ejecución			FECEJECUCION		VARCHAR2 (100 Byte)
C100_prop_medidor[101]		Propiedad medidor		PROPMEDIDOR			VARCHAR2 (100 Byte)
C010_tipo_empalme[11]		Tipo medidor 			TIPOEMPALME			Monofasico o Trifasico
							(monofásico, trifásico)
C010_tecnolgia_medidor[101]	Tecnología medidor 		TECNOLOGIA			VARCHAR2 (100 Byte)
							(electromecánico, electrónico)
C100_tipo_medicion[101]		Tipo medida 			TIPOMEDICION		VARCHAR2 (100 Byte)
							(directa, indirecta)
C045_contratista[46]		Contratista				CONTRATISTA			VARCHAR2 (45 Byte)
C001_tarea_ejecutada[2]		Tarea ejecutada			EJECUTADA			CHAR (1 Byte)
C004_tipo_tarea[5]			Tipo Tarea				TIPOTAREA			CHAR (4 Byte)
C050_material[51]			Material				MATERIAL			VARCHAR2 (50 Byte)
C004_cantidad[5]			Cantidad				CANTIDAD_MATERIAL	NUMBER (4)
C100_unidad[101]			Unidad					UNIDAD				VARCHAR2 (100 Byte)
*/
/*
C015_nro_ord_norm		Nro. NORM			NROORDEN		h6.nro_ord_norm				NroOrden		NUMBER (15)
C010_nro_suministro		Nro. Cliente		CLIENTE			h6.nro_suministro 			Cliente			NUMBER (10)
C010_tarifa				Tarifa				TARIFA			n5.campo19 					Tarifa			CHAR (10 Byte)
C100_clave_tarifa		Clave tarifa		CLAVETARIFA		n11.descripcion				ClaveTarifa		VARCHAR2 (100 Byte)
C100_prop_medidor		Propiedad medidor	PROPMEDIDOR		n11.descripcion				PropMedidor		VARCHAR2 (100 Byte)						
C123_tipo_ejecucion							TIPOEJEC		n11.valor1					TipoEjec		VARCHAR2 (20 Byte)+3+
											3+n11.descripcion							VARCHAR2 (100 Byte)
C003_capacidad			Capacidad empalme	CAPACIDAD		substr(n11.descripcion,3,3)	Capacidad		3
C010_tipo_empalme		Tipo medidor		TIPOEMPALME		-							TipoEmpalme		'Trifasico' or 'Monofasico'
C100_tipo_medicion		Tipo medida			TIPOMEDICION	n11.descripcion				TipoMedicion	VARCHAR2 (100 Byte)
C045_contratista		Contratista			CONTRATISTA		t1.nom_contratista			Contratista		VARCHAR2 (45 Byte)
C050_anormalidad							ANORMALIDAD		h3.nom_accion 				Anormalidad		VARCHAR2 (50 Byte)
C004_tipo_tarea			Tipo Tarea			TIPOTAREA		h3.cod_tarea 				TipoTarea		CHAR (4 Byte)
C001_tarea_ejecutada	Tarea ejecutada		EJECUTADA		h8.ejecutada				-				CHAR (1 Byte)
C100_comuna				Comuna				COMUNA			n11.descripcion				Comuna			VARCHAR2 (100 Byte)
C020_fecha_ejecucion	Fecha ejecución		FECEJECUCION	h6.fec_hora_fin_eje			FecEjecucion	20
*/

char C015_nro_ord_norm[16]		;		EXEC SQL VAR C015_nro_ord_norm 		IS STRING(16) 	;
char C010_nro_suministro[11]	;		EXEC SQL VAR C010_nro_suministro 	IS STRING(11) 	;
char C010_tarifa[11]			;		EXEC SQL VAR C010_tarifa 			IS STRING(11) 	;
char C100_clave_tarifa[101]		;		EXEC SQL VAR C100_clave_tarifa 		IS STRING(101) 	;
char C100_comuna[101]			;		EXEC SQL VAR C100_comuna			IS STRING(101) 	;
char C003_capacidad[4]			;		EXEC SQL VAR C003_capacidad			IS STRING(4) 	;
char C020_fecha_ejecucion[21]	;		EXEC SQL VAR C020_fecha_ejecucion	IS STRING(21) 	;
char C100_prop_medidor[101]		;		EXEC SQL VAR C100_prop_medidor 		IS STRING(101) 	;
char C010_tipo_empalme[11]		;		EXEC SQL VAR C010_tipo_empalme		IS STRING(11) 	;
char C010_tecnolgia_medidor[101];		EXEC SQL VAR C010_tecnolgia_medidor	IS STRING(101) 	;
char C100_tipo_medicion[101]	;		EXEC SQL VAR C100_tipo_medicion		IS STRING(101) 	;
char C045_contratista[46]		;		EXEC SQL VAR C045_contratista 		IS STRING(46) 	;
char C001_tarea_ejecutada[2]	;		EXEC SQL VAR C001_tarea_ejecutada 	IS STRING(2) 	;
char C004_tipo_tarea[5]			;		EXEC SQL VAR C004_tipo_tarea 		IS STRING(5) 	;
char C050_material[51]			;		EXEC SQL VAR C050_material			IS STRING(51) 	;
char C004_cantidad[5]			;		EXEC SQL VAR C004_cantidad			IS STRING(5) 	;
char C100_unidad[101]			;		EXEC SQL VAR C100_unidad			IS STRING(101) 	;


/* Variables Uso Email */
char C1024_from_name[1024]					; EXEC SQL VAR C1024_from_name			IS STRING(1024);
char c1024_from_email[1024]					; EXEC SQL VAR c1024_from_email			IS STRING(1024);
char C1024_to_name[1024]					; EXEC SQL VAR C1024_to_name			IS STRING(1024);
char C1024_to_email[1024]					; EXEC SQL VAR C1024_to_email			IS STRING(1024);
char C1024_cc_email[1024]					; EXEC SQL VAR C1024_cc_email			IS STRING(1024);
char C1024_subject[1024]					; EXEC SQL VAR C1024_subject			IS STRING(1024);
char C2048_body[2048]						; EXEC SQL VAR C2048_body				IS STRING(2048);


/* Variables generales CHAR */
char	C255_nom_file[256]		;	 		EXEC SQL VAR C255_nom_file		IS STRING(256)	;

EXEC SQL END DECLARE SECTION;

/* Variables Uso General */
int flag_email=FALSE;


/* Concatena buffer con variables incluidas                              */
long strpcat(char *str, char *msg, ...)
{
    /*double *args;*/
    va_list args;
    char buffer[2048];
    va_start(args, msg);
    vsprintf(buffer,msg,args);
    va_end(args);
    strcat(str,buffer);
    return(strlen(str));
}


/*                        ABRE CURSOR PRINCIPAL                                */
SQL_OPEN_medidores(){
    char C4000_sql_sente[4001];
	
	memset(C4000_sql_sente, '\0', sizeof(C4000_sql_sente));
	
strcpy(C4000_sql_sente, "  ");
strpcat(C4000_sql_sente, " select  ");
strpcat(C4000_sql_sente, " h6.fec_hora_ini_eje , ");
strpcat(C4000_sql_sente, " h6.nro_ord_norm NroOrden, ");
strpcat(C4000_sql_sente, " h6.nro_suministro Cliente, ");
strpcat(C4000_sql_sente, " n5.campo19 Tarifa, ");
strpcat(C4000_sql_sente, " (select n11.descripcion from nucssb0011 n11 where n11.cod_empresa = n5.cod_empresa and n11.nomtabla = 'CLATAR' and trim(n5.campo20) = n11.codigo) ClaveTarifa, ");
strpcat(C4000_sql_sente, " (select n11.descripcion from nucssb0011 n11 where n11.cod_empresa = m11.cod_empresa and n11.nomtabla = 'PROMEDI' and m11.propiedad_equipo = n11.codigo) PropMedidor, ");
strpcat(C4000_sql_sente, " (select n11.valor1 || ' - ' || n11.descripcion   ");
strpcat(C4000_sql_sente, " from hurssb0039 h39,nucssb0011 n11   ");
strpcat(C4000_sql_sente, " where h39.cod_empresa = h6.cod_empresa   ");
strpcat(C4000_sql_sente, " and h39.id_folio_preselecc = h6.id_folio_preselecc   ");
strpcat(C4000_sql_sente, " and n11.cod_empresa = h6.cod_empresa   ");
strpcat(C4000_sql_sente, " and n11.nomtabla = 'EJEINSP'  and n11.codigo=h39.tip_ejecucion) TipoEjec, ");
strpcat(C4000_sql_sente, " substr(n11.descripcion,3,3) Capacidad, ");
strpcat(C4000_sql_sente, " (case  ");
strpcat(C4000_sql_sente, "     when substr(n11.descripcion,2,1) in (' ','R','U','E','C','I') then ");
strpcat(C4000_sql_sente, "         'Trifasico' ");
strpcat(C4000_sql_sente, "     else ");
strpcat(C4000_sql_sente, "         'Monofasico' ");
strpcat(C4000_sql_sente, "     end ");
strpcat(C4000_sql_sente, " ) TipoEmpalme, ");
strpcat(C4000_sql_sente, " (select n11.descripcion  ");
strpcat(C4000_sql_sente, "  from medssb0021 m21, nucssb0011 n11  ");
strpcat(C4000_sql_sente, "  where m21.cod_empresa = m11.cod_empresa ");
strpcat(C4000_sql_sente, "  and m21.tip_aparato = m11.tip_aparato ");
strpcat(C4000_sql_sente, "  and m21.marca_equipo = m11.marca_equipo ");
strpcat(C4000_sql_sente, "  and m21.cod_modelo = m11.cod_modelo ");
strpcat(C4000_sql_sente, "  and n11.cod_empresa = m21.cod_empresa  ");
strpcat(C4000_sql_sente, "  and n11.nomtabla = 'TEGMEDI'  ");
strpcat(C4000_sql_sente, "  and n11.codigo = m21.cod_tecnologia) Tecnologia, ");
strpcat(C4000_sql_sente, " (select n11.descripcion  ");
strpcat(C4000_sql_sente, " from medssb0027 m27, nucssb0011 n11  ");
strpcat(C4000_sql_sente, " where m27.cod_empresa = m11.cod_empresa  ");
strpcat(C4000_sql_sente, " and m27.cod_funmed = substr(m11.cod_modelo,1,1) ");
strpcat(C4000_sql_sente, " and m27.cod_empresa = n11.cod_empresa ");
strpcat(C4000_sql_sente, " and n11.nomtabla='CODMEDI'  ");
strpcat(C4000_sql_sente, " and trim(n11.codigo) = trim(m27.cod_medida)) TipoMedicion, ");
strpcat(C4000_sql_sente, " (select t1.nom_contratista from tasssb0001 t1 where t1.cod_empresa = h6.cod_empresa and t1.cod_contratista = h6.cod_contratista) Contratista, ");
strpcat(C4000_sql_sente, " h3.nom_accion Anormalidad, ");
strpcat(C4000_sql_sente, " h3.cod_tarea TipoTarea,  ");
strpcat(C4000_sql_sente, " h8.ejecutada, ");
strpcat(C4000_sql_sente, " (select h4.nom_material from hurssb0004 h4 where h4.cod_empresa = h12.cod_empresa and h4.cod_material = h12.cod_material) Material, ");
strpcat(C4000_sql_sente, " h12.cantidad_material, ");
strpcat(C4000_sql_sente, " (select n11.descripcion from hurssb0004 h4, nucssb0011 n11 where h4.cod_empresa = h12.cod_empresa and h4.cod_material = h12.cod_material ");
strpcat(C4000_sql_sente, " and h4.cod_empresa = n11.cod_empresa and h4.unidad= n11.codigo and n11.nomtabla = 'UNIMAT') Unidad ");
strpcat(C4000_sql_sente, " from hurssb0006 h6, hurssb0008 h8, nucssb0005 n5, hurssb0003 h3,medssb0011 m11, nucssb0011 n11,hurssb0012 h12 ");
strpcat(C4000_sql_sente, " where h6.cod_empresa = %s  " , C003_par_empresa);
strpcat(C4000_sql_sente, " and h6.tip_ord_norm = 'O' ");
strpcat(C4000_sql_sente, " and h6.fec_hora_ini_eje >= to_date('%s' || ' 00:00:00' , 'dd/mm/yyyy hh24:mi:ss') ", C010_par_fec_inicio);
strpcat(C4000_sql_sente, " and h6.fec_hora_ini_eje <= to_date('%s' || ' 23:59:59' , 'dd/mm/yyyy hh24:mi:ss') ", C010_par_fec_fin);
strpcat(C4000_sql_sente, " and h6.est_orden = 'F' ");
strpcat(C4000_sql_sente, " and h6.cod_empresa = h8.cod_empresa ");
strpcat(C4000_sql_sente, " and h6.nro_ord_norm = h8.nro_ord_norm ");
strpcat(C4000_sql_sente, " and h6.tip_ord_norm = h8.tip_ord_norm ");
strpcat(C4000_sql_sente, " and h8.cod_empresa = h3.cod_empresa ");
strpcat(C4000_sql_sente, " and h8.cod_accion = h3.cod_accion ");
strpcat(C4000_sql_sente, " and h6.nro_suministro = n5.nro_suministro ");
strpcat(C4000_sql_sente, " and h6.cod_empresa = n5.cod_empresa ");
strpcat(C4000_sql_sente, " and h6.nro_suministro  = m11.nro_suministro  ");
strpcat(C4000_sql_sente, " and h6.cod_empresa = m11.cod_empresa ");
strpcat(C4000_sql_sente, " and m11.est_aparato = 'I' ");
strpcat(C4000_sql_sente, " and m11.tip_aparato = 'ME' ");
strpcat(C4000_sql_sente, " and n11.cod_empresa = n5.cod_empresa  ");
strpcat(C4000_sql_sente, " and n11.codigo = n5.tip_empalme ");
strpcat(C4000_sql_sente, " and n11.nomtabla = 'EMPAL' ");
strpcat(C4000_sql_sente, " and h6.cod_empresa = h12.cod_empresa ");
strpcat(C4000_sql_sente, " and h6.nro_ord_norm = h12.nro_ord_norm ");
strpcat(C4000_sql_sente, " and h6.tip_ord_norm = h12.tip_ord_norm ");

	
	/*strcpy(C4000_sql_sente, " select  ");
	strpcat(C4000_sql_sente, " h6.nro_ord_norm NroOrden, ");
	strpcat(C4000_sql_sente, " h6.nro_suministro Cliente, ");
	strpcat(C4000_sql_sente, " n5.campo19 Tarifa, ");
	strpcat(C4000_sql_sente, " (select n11.descripcion from nucssb0011 n11 where n11.cod_empresa = n5.cod_empresa and n11.nomtabla = 'CLATAR' and trim(n5.campo20) = n11.codigo) ClaveTarifa, ");
	strpcat(C4000_sql_sente, " (select n11.descripcion from nucssb0011 n11 where n11.cod_empresa = m11.cod_empresa and n11.nomtabla = 'PROMEDI' and m11.propiedad_equipo = n11.codigo) PropMedidor, ");
	strpcat(C4000_sql_sente, "    (select n11.valor1 || ' - ' || n11.descripcion   ");
	strpcat(C4000_sql_sente, "    from hurssb0039 h39,nucssb0011 n11   ");
	strpcat(C4000_sql_sente, "    where h39.cod_empresa = h6.cod_empresa   ");
	strpcat(C4000_sql_sente, "    and h39.id_folio_preselecc = h6.id_folio_preselecc   ");
	strpcat(C4000_sql_sente, "    and n11.cod_empresa = h6.cod_empresa   ");
	strpcat(C4000_sql_sente, "    and n11.nomtabla = 'EJEINSP'  and n11.codigo=h39.tip_ejecucion) TipoEjec, ");
	strpcat(C4000_sql_sente, " substr(n11.descripcion,3,3) Capacidad, ");
	strpcat(C4000_sql_sente, "    (case  ");
	strpcat(C4000_sql_sente, "        when substr(n11.descripcion,2,1) in (' ','R','U','E','C','I') then ");
	strpcat(C4000_sql_sente, "            'Trifasico' ");
	strpcat(C4000_sql_sente, "        else ");
	strpcat(C4000_sql_sente, "            'Monofasico' ");
	strpcat(C4000_sql_sente, "        end ");
	strpcat(C4000_sql_sente, "    ) TipoEmpalme, ");
	strpcat(C4000_sql_sente, "    (select n11.descripcion  ");
	strpcat(C4000_sql_sente, "    from medssb0027 m27, nucssb0011 n11  ");
	strpcat(C4000_sql_sente, "    where m27.cod_empresa = m11.cod_empresa  ");
	strpcat(C4000_sql_sente, "    and m27.cod_funmed = substr(m11.cod_modelo,1,1) ");
	strpcat(C4000_sql_sente, "    and m27.cod_empresa = n11.cod_empresa ");
	strpcat(C4000_sql_sente, "    and n11.nomtabla='CODMEDI'  ");
	strpcat(C4000_sql_sente, "    and trim(n11.codigo) = trim(m27.cod_medida)) TipoMedicion, ");
	strpcat(C4000_sql_sente, " (select t1.nom_contratista from tasssb0001 t1 where t1.cod_empresa = h6.cod_empresa and t1.cod_contratista = h6.cod_contratista) Contratista, ");
	strpcat(C4000_sql_sente, " h3.nom_accion Anormalidad, ");
	strpcat(C4000_sql_sente, " h3.cod_tarea TipoTarea,  ");
	strpcat(C4000_sql_sente, " h8.ejecutada, ");
	strpcat(C4000_sql_sente, " (select n11.descripcion from nucssb0011 n11 where n11.cod_empresa = n19.cod_empresa and n11.nomtabla = 'COMUNA' and n19.campo22 = n11.codigo) Comuna, ");
	strpcat(C4000_sql_sente, " to_char(h6.fec_hora_fin_eje ,'dd/mm/yyyy hh24:mi:ss') FecEjecucion ");
	strpcat(C4000_sql_sente, " from hurssb0006 h6, hurssb0008 h8, nucssb0005 n5, hurssb0003 h3,medssb0011 m11, nucssb0011 n11,nucssb0019 n19 ");
	strpcat(C4000_sql_sente, " where h6.cod_empresa = %s  " , C003_par_empresa);
	strpcat(C4000_sql_sente, "    and h6.tip_ord_norm = 'O' ");
	strpcat(C4000_sql_sente, "    and h6.fec_hora_ini_eje >= to_date('%s' || ' 00:00:00' , 'dd/mm/yyyy hh24:mi:ss') ", C010_par_fec_inicio);
	strpcat(C4000_sql_sente, "    and h6.fec_hora_ini_eje <= to_date('%s' || ' 23:59:59' , 'dd/mm/yyyy hh24:mi:ss') ", C010_par_fec_fin);
	strpcat(C4000_sql_sente, "    and h6.est_orden = 'F' ");
	strpcat(C4000_sql_sente, "    and h6.cod_empresa = h8.cod_empresa ");
	strpcat(C4000_sql_sente, "    and h6.nro_ord_norm = h8.nro_ord_norm ");
	strpcat(C4000_sql_sente, "    and h6.tip_ord_norm = h8.tip_ord_norm ");
	strpcat(C4000_sql_sente, "    and h8.cod_empresa = h3.cod_empresa ");
	strpcat(C4000_sql_sente, "    and h8.cod_accion = h3.cod_accion ");
	strpcat(C4000_sql_sente, "    and h6.nro_suministro = n5.nro_suministro ");
	strpcat(C4000_sql_sente, "    and h6.cod_empresa = n5.cod_empresa ");
	strpcat(C4000_sql_sente, "    and h6.nro_suministro  = m11.nro_suministro  ");
	strpcat(C4000_sql_sente, "    and h6.cod_empresa = m11.cod_empresa ");
	strpcat(C4000_sql_sente, "    and m11.est_aparato = 'I' ");
	strpcat(C4000_sql_sente, "    and m11.tip_aparato = 'ME' ");
	strpcat(C4000_sql_sente, "    and n11.cod_empresa = n5.cod_empresa  ");
	strpcat(C4000_sql_sente, "    and n11.codigo = n5.tip_empalme ");
	strpcat(C4000_sql_sente, "    and n11.nomtabla = 'EMPAL' ");
	strpcat(C4000_sql_sente, "    and n11.estado = 'A'  ");
	strpcat(C4000_sql_sente, "    and h6.nro_suministro  = n19.nro_suministro  ");
	strpcat(C4000_sql_sente, "    and h6.cod_empresa = n19.cod_empresa  ");
	strpcat(C4000_sql_sente, "    and n19.tip_dir = 'S'  ");
	
	if (strcmp(C010_par_tipo_ejecucion,"TOD")!=0){
		strpcat(C4000_sql_sente, "    and exists ");
		strpcat(C4000_sql_sente, "    (select 1 ");
		strpcat(C4000_sql_sente, "    from hurssb0039 h39 ");
		strpcat(C4000_sql_sente, "    where h39.cod_empresa = h6.cod_empresa ");
		strpcat(C4000_sql_sente, "    and h39.id_folio_preselecc = h6.id_folio_preselecc  ");
		strpcat(C4000_sql_sente, "    and h39.tip_ejecucion = '%s' ) " , C010_par_tipo_ejecucion);
	}
		
	if (strcmp(C010_par_origen,"TOD")!=0){
		strpcat(C4000_sql_sente, "    and h6.TIP_ORD_ORIGEN = '%s'  " , C010_par_origen);
	}
		
	strpcat(C4000_sql_sente, " order by h6.nro_ord_norm ");
	*/
	
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
	memset(C010_nro_suministro, '\0', sizeof(C010_nro_suministro));
	memset(C010_tarifa, '\0', sizeof(C010_tarifa));
	memset(C100_clave_tarifa, '\0', sizeof(C100_clave_tarifa));
	memset(C100_prop_medidor, '\0', sizeof(C100_prop_medidor));
	memset(C123_tipo_ejecucion, '\0', sizeof(C123_tipo_ejecucion));
	memset(C003_capacidad, '\0', sizeof(C003_capacidad));
	memset(C010_tipo_empalme, '\0', sizeof(C010_tipo_empalme));
	memset(C100_tipo_medicion, '\0', sizeof(C100_tipo_medicion));
	memset(C045_contratista, '\0', sizeof(C045_contratista));
	memset(C050_anormalidad, '\0', sizeof(C050_anormalidad));
	memset(C004_tipo_tarea, '\0', sizeof(C004_tipo_tarea));
	memset(C001_tarea_ejecutada, '\0', sizeof(C001_tarea_ejecutada));
	memset(C100_comuna, '\0', sizeof(C100_comuna));
	memset(C020_fecha_ejecucion, '\0', sizeof(C020_fecha_ejecucion));

    EXEC SQL 
		FETCH cur_medidores 
		INTO :C015_nro_ord_norm,:C010_nro_suministro,:C010_tarifa,:C100_clave_tarifa,:C100_prop_medidor
		,:C123_tipo_ejecucion,:C003_capacidad,:C010_tipo_empalme,:C100_tipo_medicion,:C045_contratista
		,:C050_anormalidad,:C004_tipo_tarea,:C001_tarea_ejecutada,:C100_comuna,:C020_fecha_ejecucion;

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

/* Crea archivo de salida en base a los prefijos y la ruta que recupera de NUCSSB0044     */
int bfnCrearArchivoSalida(FILE **fpOut, char *prefix1, char *prefix2, char *ext){/**/
	int		iRet;
	char	C256_pat_unix[256]	; EXEC SQL VAR C256_pat_unix   IS STRING(256);
	char	C020_fecha[20]		; EXEC SQL VAR C020_fecha	   IS STRING(20);
	
	memset(C256_pat_unix, '\0', sizeof(C256_pat_unix));
	memset(C020_fecha, '\0', sizeof(C020_fecha));
	memset(C255_nom_file, '\0', sizeof(C255_nom_file));

	/* Obtiene path unix */
	EXEC SQL 
		SELECT trim(pat_unix)
		INTO   :C256_pat_unix
		FROM   NUCSSB0044
		WHERE  NUCSSB0044.COD_EMPRESA         = :C003_par_empresa  
		AND    trim(NUCSSB0044.COD_SISTEMA)   = 'PERD'
		AND    trim(NUCSSB0044.CEN_OPERATIVO) = 'TOD'
		AND    trim(NUCSSB0044.TIP_PATH)      = 'LIST';
    iRet = do_error("Select NUCSSB0044");
    if ( iRet == TRUE )
        return ( FALSE );

	/* Obtiene fecha del sistema */
    EXEC SQL
         SELECT  TO_CHAR( sysdate, 'ddmmyyyy_hh24mi' )
         INTO    :C020_fecha
         FROM    DUAL;
    iRet = do_error("Select SYSDATE");
    if ( iRet == TRUE )
        return ( FALSE );

	/* Crea archivo de salida */
	sprintf(C255_nom_file, "%s%s%s_%s.%s", C256_pat_unix,prefix1,prefix2,C020_fecha,ext);
	if( ( *fpOut = fopen( C255_nom_file, "w+" ) ) == NULL )
	{
		printf("ERR|Error al Generar Archivo <%s>. Error %s\n", 
			   C255_nom_file, strerror(errno));
		return ( FALSE );
	}

	/* Debugger */
	if(DEBUG){
		printf("------------------------------------------------------\n");
		printf("DEBUG[ifnCrearArchivoSalida]\n");
		printf("Resultado [%s] OK\n",C255_nom_file);
		printf("------------------------------------------------------\n\n");
	}

	return ( TRUE );
}

/* Agrega contenido al archivo de salida                                     */
int bfnAgregarArchivoSalida(FILE *fpOut, char *cBuffer){/**/
	int iRet = TRUE;

	fprintf(fpOut, "%s",cBuffer);

	/* Debugger */
	if(DEBUG){
		printf("------------------------------------------------------\n");
		printf("DEBUG[ifnAgregarArchivoSalida]\n");
		printf("[%s\n]",cBuffer);
		printf("Resultado OK\n");
		printf("------------------------------------------------------\n\n");
	}

	return ( TRUE );
}

/* Cierra archivo de salida                                                  */
int bfnCerrarArchivoSalida(FILE **fpOut){/**/
	fclose ( *fpOut );

	return ( TRUE );
}


/* Obtiene los parametros desde rol.email ademas los datos duros     */
int SQL_parametros_main_email(){

	EXEC SQL 
		SELECT DISTINCT EMAIL
		INTO	:C1024_to_email
		FROM 	ROL
		WHERE 	ROL = :C007_par_rol;
	do_error("Select ROL - SQL_parametros_main_email()");
		
	if (SQLNOTFOUND){flag_email=FALSE;}
	else{flag_email=TRUE;}
		
	strcpy(C1024_from_name, "noreply");
	strcpy(c1024_from_email, "no-reply@chilectra.cl");
	strcpy(C1024_subject, "Informe Tiempos de Atención");
	strpcat(C2048_body, "Estimado Usuario.\nHa finalizado la ejecución del Informe. Se generó archivo %s \n", C255_nom_file);
		
		if(DEBUG){
			printf("------------------------------------------------------\n");
			printf("DEBUG[SQL_parametros_main_email]\n");
			printf("Resultado Query: %s,%s,%s,%s,%s\n",C1024_from_name,c1024_from_email,C1024_to_email,C1024_subject,C2048_body);
			printf("------------------------------------------------------\n\n");
		}
		
	return ( TRUE );
}

/* Obtiene parametros del correo                                     */
int SQL_parametros_email(){

	memset(C1024_from_name, '\0', sizeof(C1024_from_name));
	memset(c1024_from_email, '\0', sizeof(c1024_from_email));
	memset(C1024_to_name, '\0', sizeof(C1024_to_name));
	memset(C1024_to_email, '\0', sizeof(C1024_to_email));
	memset(C1024_cc_email, '\0', sizeof(C1024_cc_email));
	memset(C1024_subject, '\0', sizeof(C1024_subject));
	memset(C2048_body, '\0', sizeof(C2048_body));

	SQL_parametros_main_email();
	
	if(!flag_email) return ( FALSE );

	return ( TRUE );
}

/* Envia email segun parametros de configuracion                     */
int ifnSendEmail()
{
	if(!SQL_parametros_email())
		return ( FALSE );

	correo_head(C1024_from_name, c1024_from_email, C1024_to_name, C1024_to_email, C1024_cc_email, C1024_subject);
	correo_body(C2048_body);

	if (!correo_enviar())
	{
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


/* ------------------------------------------------------------------------- */
/*                        PROCESAMIENTO DE DATOS                             */
/* ------------------------------------------------------------------------- */
int bfnProcesar(){
	FILE	*fpMedidores=NULL;
	char    C5000_Buffer[5001]; 	EXEC SQL VAR C5000_Buffer IS STRING(5001) ;
	int iLecturas = 0;
	int i = 0;

	if (SQL_OPEN_medidores())
	{
		while (SQL_FETCH_medidores())
		{
			memset(C5000_Buffer, '\0', sizeof(C5000_Buffer));
			if(!flag_records)/*controla si hay registros*/
			{
				flag_records=TRUE;
				if(!bfnCrearArchivoSalida(&fpMedidores,"Norm","Material","xls"))
				{
					return ( FALSE );
				}

				/* Encabezado archivo */
				strpcat(C5000_Buffer,"%-16.16s","Nro. NORM");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-13.13s","Nro. Cliente");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-11.11s","Tarifa");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-101.101s","Clave tarifa");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-101.101s","Comuna");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-18.18s","Capacidad empalme");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-21.21s","Fecha ejecución");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-101.101s","Propiedad medidor");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-13.13s","Tipo medidor");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-101.101s","Tipo medida");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-46.46s","Contratista");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-16.16s","Tarea ejecutada");
				strpcat(C5000_Buffer,"%c",delimiter);
				strpcat(C5000_Buffer,"%-11.11s","Tipo Tarea");

				strpcat(C5000_Buffer,"%s","\n");
			}
			
			/* Archivo de lecturas */
			strpcat(C5000_Buffer,"%-16.16s",C015_nro_ord_norm);
			strpcat(C5000_Buffer,"%c",delimiter);
			strpcat(C5000_Buffer,"%-13.13s",C010_nro_suministro);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-11.11s",C010_tarifa);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-101.101s",C100_clave_tarifa);
			strpcat(C5000_Buffer,"%c",delimiter);					
			strpcat(C5000_Buffer,"%-101.101s",C100_comuna);
			strpcat(C5000_Buffer,"%c",delimiter);					
			strpcat(C5000_Buffer,"%-18.18s",C003_capacidad);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-21.21s",C020_fecha_ejecucion);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-101.101s",C100_prop_medidor);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-13.13s",C010_tipo_empalme);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-101.101s",C100_tipo_medicion);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-46.46s",C045_contratista);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-16.16s",C001_tarea_ejecutada);
			strpcat(C5000_Buffer,"%c",delimiter);			
			strpcat(C5000_Buffer,"%-11.11s",C004_tipo_tarea);

			strpcat(C5000_Buffer,"%s","\n");
			
			bfnAgregarArchivoSalida(fpMedidores,C5000_Buffer);
			/* Fin archivo de normalizaciones */			
				
		}
		
		if(flag_records)
		{
			bfnCerrarArchivoSalida(&fpMedidores);
			ifnSendEmail();
		}
	
	}
		
	return ( TRUE );
}

/* ------------------------------------------------------------------------- */
/*                        PROGRAMA  PRINCIPAL                                */
/* ------------------------------------------------------------------------- */
main(int argc,char **argv)
{
   if( argc != 6){
		printf("Use : %s <N° Conexion> <Empresa> <Rol> <FecInicio> <FecFin> \n",argv[0]);
		exit(1);
    }

	strcpy( C001_par_conexion,			argv[1] );
	strcpy( C003_par_empresa,			argv[2] );
	strcpy( C007_par_rol,				argv[3] );
	strcpy( C010_par_fec_inicio,		argv[4] );
	strcpy( C010_par_fec_fin,			argv[5] );

	sql_conexion(C001_par_conexion);

	if(!bfnProcesar()){
		printf("Error\n");
		exit(1);
	}else{
		printf("OK\n");
		exit(0);
	}
}               
