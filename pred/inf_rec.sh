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

FECHAESPECIAL=`date '+%d/%m/%Y'`

#Valido si es domingo o feriado
resultado=`sqlplus -s 'clientes4j/producto@DESAESVAL' << EOF
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


echo "resultado: ${resultado}"

if [ $resultado == '1' ]; then

    echo "NO EJECUTA, DIA FERIADO"

else
    echo "EJECUTANDO, DIA NORMAL"
fi

echo [FIN]
#date

