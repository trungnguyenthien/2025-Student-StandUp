#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

class DFPlayerController {
private:
    SoftwareSerial *serial;
    DFRobotDFPlayerMini player;
    const uint8_t RX_PIN, TX_PIN;

public:
    // Khởi tạo với chân RX/TX tùy chỉnh
    DFPlayerController(uint8_t rxPin, uint8_t txPin) : RX_PIN(rxPin), TX_PIN(txPin) {
        serial = new SoftwareSerial(RX_PIN, TX_PIN);
        serial->begin(9600);
        
        if (!player.begin(*serial)) {
            Serial.println("Lỗi kết nối DFPlayer!");
            while (true); // Dừng chương trình nếu lỗi
        }
        delay(2000); // Chờ module khởi động
    }

    // Phát file theo số thứ tự
    void play(uint16_t fileNumber) {
        player.play(fileNumber);
    }

    // Phát file trong thư mục (vd: playFolder(1, 5) → SD:/01/005.mp3)
    void playFolder(uint8_t folder, uint8_t file) {
        player.playFolder(folder, file);
    }

    // Dừng/phát lại
    void pause() { player.pause(); }
    void resume() { player.start(); }

    // Điều chỉnh âm lượng (0-30)
    void setVolume(uint8_t volume) {
        player.volume(volume);
    }

    // Chuyển bài tiếp theo/trước đó
    void nextTrack() { player.next(); }
    void previousTrack() { player.previous(); }
};