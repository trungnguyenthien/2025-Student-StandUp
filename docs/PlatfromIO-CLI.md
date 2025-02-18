
Lệnh xem monitor
```
pio device monitor
```

List port (liệt kê các cổng nối tiếp):
```
pio device list
```

List device (xem thông tin chi tiết của một thiết bị cụ thể):
```
pio device info --port <your_port>
```

Reset board:
```
pio device reset --port /dev/cu.usbserial-14330
```

Build + Upload + Monitor:
```
pio run --target upload && pio device monitor --port <your_port>
```

Upload với log
```
pio run --target upload -v
```