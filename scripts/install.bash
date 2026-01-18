# #!/bin/bash
set -x

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_STATS_PATH="${HOME}/app/stats"

sudo apt install cmake ccache libsensors4-dev

cd ${PROJECT_STATS_PATH}
cmake ${PROJECT_STATS_PATH}
cmake --build ${PROJECT_STATS_PATH}

mkdir ${PROJECT_STATS_PATH}/tmp
ln -s /sys/class/power_supply ${PROJECT_STATS_PATH}/tmp/BAT0

cd ${SCRIPT_DIR}

## Отключаем все сервисы, если они были ранее установлены и запущены
bash ${PROJECT_STATS_PATH}/scripts/disable.bash

#Create service

SERVICE_STATS_NAME="stats.service"
SERVICE_STATS_PATH="/etc/systemd/system/$SERVICE_STATS_NAME"

sudo bash -c "cat <<EOL > $SERVICE_STATS_PATH
[Unit]
Description=stats
StartLimitBurst=10
StartLimitIntervalSec=60

[Service]
Type=simple
WorkingDirectory=$PROJECT_STATS_PATH/
Restart=always
ExecStart=$PROJECT_STATS_PATH/stats -u -4 127.0.0.1 10000
RestartSec=3

[Install]
WantedBy=default.target
EOL"

bash ${PROJECT_STATS_PATH}/scripts/enable.bash
