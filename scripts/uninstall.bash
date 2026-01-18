## Отключаем все сервисы, если они были ранее установлены и запущены
PROJECT_STATS_PATH=${HOME}/app/stats

rm -rf ${PROJECT_STATS_PATH}/tmp

bash ${PROJECT_STATS_PATH}/scripts/disable.bash

sudo rm /etc/systemd/system/stats.service
