Cài USB driver:
- CP2102: https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads
- CH340: 

Lệnh clean

```
rm -rf ~/.platformio/.cache
pio run --target clean
sudo killall -9 pioagent
sudo killall -9 python
```