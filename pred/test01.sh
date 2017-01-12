#!/bin/bash

# # Formato de fecha
# formato_fecha () {
#     fecha_ultimo_proceso=`cat fecha_inf_rec.txt`
#     echo "fecha_ultimo_proceso: ${fecha_ultimo_proceso}"

# }

fecha_es='20170101  +%Y%m%d '

user_date=$( date -d $fecha_es )
    
echo ${user_date '+%Y%m%d'}