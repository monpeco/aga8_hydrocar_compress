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
echo "Fecha: ${fecha_proceso}"

