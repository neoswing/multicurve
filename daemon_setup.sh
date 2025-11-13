#!/bin/bash
echo "Compiling multicurve daemon..."
cmake --build .

# Install to system
sudo cp multicurve_mainframe /usr/local/bin/
sudo chmod +x /usr/local/bin/multicurve_mainframe

# Systemd service file
sudo tee /etc/systemd/system/multicurve.service > /dev/null <<EOF
[Unit]
Description= Multicurve Daemon Service
After=network.target

[Service]
Type=simple
ExecStart=/usr/local/bin/multicurve_mainframe
Restart=always
RestartSec=5
User=root
Group=root
WorkingDirectory=/tmp
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

echo "Enabling service..."
sudo systemctl daemon-reload
sudo systemctl enable multicurve
sudo systemctl start multicurve

sudo systemctl enable nginx
sudo systemctl start nginx

sudo systemctl status multicurve
echo "View logs: sudo journalctl -u multicurve -f"
