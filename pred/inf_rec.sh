#/bin/bash
export ORACLE_BASE=/u01/app/oracle/product/11.2.0/client_1
export ORACLE_HOME=/u01/app/oracle/product/11.2.0/client_1
export PATH=.:$HOME/bin:$ORACLE_HOME/bin:/usr/lib:$PATH
export LD_LIBRARY_PATH=$ORACLE_HOME/lib:/usr/lib
export SHLIB_PATH=$ORACLE_HOME/lib
export NLS_LANG=SPANISH_SPAIN.WE8MSWIN1252
export ORACLE_SID=prodesval
export TWO_TASK=prodesval

echo [INICIO]
date

fecha_proceso=$1
echo "fecha_proceso: ${fecha_proceso}"

#Valido si es domingo o feriado
es_feriado=`sqlplus -s 'clientes4j/producto@DESAESVAL' << EOF
set serveroutput on
set feedback off
set head off
select nvl(max(xxx),0)
from (select 1 xxx
      from nucssb0054
      where cod_empresa in (1,2)
        and est_registro = 'A'
        and cod_sistema = 'AIC'
        and fecha  = trunc(sysdate)
      union
      select 1
      from dual
      where to_char(trunc(sysdate), 'DY', 'NLS_DATE_LANGUAGE=SPANISH') = 'DOM');
exit;
EOF`


echo "es_feriado: ${es_feriado}"

if [ $es_feriado == '0' ]; then
    echo "EJECUTANDO, DIA NORMAL"

else
    echo "NO EJECUTA, DIA FERIADO"

fi

echo [FIN]
#date

