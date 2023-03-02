/*
 *  Please add lib ESP32-audioI2S to your Arduino IDE:
 * https://github.com/schreibfaul1/ESP32-audioI2S/wiki
 */
#include "M5StickCPlus.h"
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"

// Digital I/O used
#define I2S_DOUT 25
#define I2S_BCLK 26
#define I2S_LRC  0

Audio audio;

String ssid     = "*******";
String password = "*******";

void setup() {
    M5.begin();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21);  // default 0...21
                          //  or alternative
                          //  audio.setVolumeSteps(64); // max 255
                          //  audio.setVolume(63);
                          //

    audio.connecttohost(
        "http://vis.media-ice.musicradio.com/CapitalMP3");  // web radio
    // audio.connecttohost("http://iskatel.hostingradio.ru:8015/iskatel-320.aac");
    // // aac
    //  audio.connecttohost("http://mcrscast.mcr.iol.pt/cidadefm"); // mp3
    //  audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u"); //
    //  m3u audio.connecttohost("https://stream.srg-ssr.ch/rsp/aacp_48.asx"); //
    //  asx audio.connecttohost("http://tuner.classical102.com/listen.pls"); //
    //  pls audio.connecttohost("http://stream.radioparadise.com/flac"); // flac
    //  audio.connecttohost("http://stream.sing-sing-bis.org:8000/singsingFlac");
    //  // flac (ogg)
    //  audio.connecttohost("http://s1.knixx.fm:5347/dein_webradio_vbr.opus");
    //  // opus (ogg)
    //  audio.connecttohost("http://26373.live.streamtheworld.com:3690/XHQQ_FMAAC/HLSTS/playlist.m3u8");
    //  // HLS
    //  audio.connecttohost("http://eldoradolive02.akamaized.net/hls/live/2043453/eldorado/master.m3u8");
    //  // HLS (ts) audio.connecttoFS(SD, "/test.wav");     // SD
    //  audio.connecttoFS(SD_MMC, "/test.wav"); // SD_MMC
    //  audio.connecttoFS(SPIFFS, "/test.wav"); // SPIFFS
    //  audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe
    //  stehlen.", "de"); // Google TTS
}

void loop() {
    audio.loop();
}

// optional
void audio_info(const char *info) {
    Serial.print("info        ");
    Serial.println(info);
}
void audio_id3data(const char *info) {  // id3 metadata
    Serial.print("id3data     ");
    Serial.println(info);
}
void audio_eof_mp3(const char *info) {  // end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);
}
void audio_showstation(const char *info) {
    Serial.print("station     ");
    Serial.println(info);
}
void audio_showstreamtitle(const char *info) {
    Serial.print("streamtitle ");
    Serial.println(info);
}
void audio_bitrate(const char *info) {
    Serial.print("bitrate     ");
    Serial.println(info);
}
void audio_commercial(const char *info) {  // duration in sec
    Serial.print("commercial  ");
    Serial.println(info);
}
void audio_icyurl(const char *info) {  // homepage
    Serial.print("icyurl      ");
    Serial.println(info);
}
void audio_lasthost(const char *info) {  // stream URL played
    Serial.print("lasthost    ");
    Serial.println(info);
}
void audio_eof_speech(const char *info) {
    Serial.print("eof_speech  ");
    Serial.println(info);
}
