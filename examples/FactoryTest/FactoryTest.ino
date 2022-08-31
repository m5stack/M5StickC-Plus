#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <driver/i2s.h>
#include <driver/rmt.h>
#include <rom/crc.h>

#include "M5StickCPlus.h"
#include "esp_pm.h"
#include "fft.h"

typedef struct {
    double x;
    double y;
    double z;
} point_3d_t;

typedef struct {
    point_3d_t start_point;
    point_3d_t end_point;
} line_3d_t;

typedef struct {
    double x;
    double y;
} point_2d_t;

double r_rand = PI / 180;

double r_alpha = 19.47 * PI / 180;
double r_gamma = 20.7 * PI / 180;

double sin_alpha = sin(19.47 * PI / 180);
double cos_alpha = cos(19.47 * PI / 180);
double sin_gamma = sin(20.7 * PI / 180);
double cos_gamma = cos(20.7 * PI / 180);

extern const unsigned char ImageData[768];
extern const unsigned char error_48[4608];
extern const unsigned char icon_ir[4608];
extern const unsigned char icon_ble[4608];
extern const unsigned char icon_wifi[4608];
extern const unsigned char icon_ble_disconnect[4608];

bool TestMode = false;

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

hw_timer_t *timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux       = portMUX_INITIALIZER_UNLOCKED;
volatile uint8_t TimerCount = 0;

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    digitalWrite(10, TimerCount % 100);
    TimerCount++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void checkAXPPress() {
    if (M5.Axp.GetBtnPress()) {
        do {
            delay(20);
        } while (M5.Axp.GetBtnPress());
        M5.Beep.mute();
        ESP.restart();
    }
}

void Displaybuff() {
    if (TestMode) {
        Disbuff.setTextSize(1);
        Disbuff.setTextColor(TFT_RED);
        Disbuff.drawString("Test Mode", 0, 0, 1);
        Disbuff.setTextColor(TFT_WHITE);
    }
    Disbuff.pushSprite(0, 0);
}

void ErrorDialog(uint8_t code, const char *str) {
    Disbuff.fillRect(28, 20, 184, 95, Disbuff.color565(45, 45, 45));
    Disbuff.fillRect(30, 22, 180, 91, TFT_BLACK);
    // Disbuff.drawRect(30,22,180,91,Disbuff.color565(45,45,45));
    Disbuff.setSwapBytes(true);
    Disbuff.pushImage(40, 43, 48, 48, (uint16_t *)error_48);

    Disbuff.setCursor(145, 37);
    Disbuff.setTextFont(2);
    Disbuff.printf("%02X", code);
    Disbuff.drawString("ERROR", 55 + 45, 10 + 27, 2);
    Disbuff.drawString("-----------------", 55 + 45, 30 + 27, 1);
    Disbuff.drawString(str, 55 + 45, 45 + 27, 1);
    Disbuff.drawString("check Hardware ", 55 + 45, 60 + 27, 1);
    Disbuff.pushSprite(0, 0);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);
    Disbuff.setTextFont(1);
}

bool point3Dto2D(point_3d_t *source, point_2d_t *point) {
    point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
    point->y = -(source->x * sin_gamma * sin_alpha) -
               (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
    return true;
}

bool point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y) {
    *x = point->x + 120;
    *y = 67 - point->y;
    return true;
}

bool printLine3D(TFT_eSprite *display, line_3d_t *line, uint32_t color) {
    uint8_t start_x, start_y, end_x, end_y;
    point_2d_t point;
    point3Dto2D(&line->start_point, &point);
    point2DToDisPoint(&point, &start_x, &start_y);
    point3Dto2D(&line->end_point, &point);
    point2DToDisPoint(&point, &end_x, &end_y);

    display->drawLine(start_x, start_y, end_x, end_y, color);

    return true;
}

void RotatePoint(point_3d_t *point, double x, double y, double z) {
    if (x != 0) {
        point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0) {
        point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0) {
        point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

void RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y,
                 double z) {
    if (x != 0) {
        point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0) {
        point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0) {
        point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

line_3d_t rect[12] = {
    {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
    {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
    {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
    {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
    {
        .start_point = {-1, -1, 1},
        .end_point   = {-1, -1, -1},
    },
    {
        .start_point = {1, -1, 1},
        .end_point   = {1, -1, -1},
    },
    {
        .start_point = {1, 1, 1},
        .end_point   = {1, 1, -1},
    },
    {
        .start_point = {-1, 1, 1},
        .end_point   = {-1, 1, -1},
    },
    {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
    {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
    {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
    {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
};

void MPU6886Test() {
    float accX = 0;
    float accY = 0;
    float accZ = 0;

    double theta = 0, last_theta = 0;
    double phi = 0, last_phi = 0;
    double alpha = 0.2;

    line_3d_t x = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
    line_3d_t y = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
    line_3d_t z = {.start_point = {0, 0, 0}, .end_point = {0, 0, 30}};

    line_3d_t rect_source[12];
    line_3d_t rect_dis;
    for (int n = 0; n < 12; n++) {
        rect_source[n].start_point.x = rect[n].start_point.x * 30;
        rect_source[n].start_point.y = rect[n].start_point.y * 30;
        rect_source[n].start_point.z = rect[n].start_point.z * 30;
        rect_source[n].end_point.x   = rect[n].end_point.x * 30;
        rect_source[n].end_point.y   = rect[n].end_point.y * 30;
        rect_source[n].end_point.z   = rect[n].end_point.z * 30;
    }

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        M5.Imu.getAccelData(&accX, &accY, &accZ);
        // M5.MPU6886.getAccelData(&accX, &accY, &accZ);
        if ((accX < 1) && (accX > -1)) {
            theta = asin(-accX) * 57.295;
        }
        if (accZ != 0) {
            phi = atan(accY / accZ) * 57.295;
        }

        theta = alpha * theta + (1 - alpha) * last_theta;
        phi   = alpha * phi + (1 - alpha) * last_phi;

        Disbuff.fillRect(0, 0, 240, 135, TFT_BLACK);
        Disbuff.setTextSize(1);
        Disbuff.setCursor(10, 115);
        Disbuff.printf("%.2f", theta);
        Disbuff.setCursor(10, 125);
        Disbuff.printf("%.2f", phi);
        // Displaybuff();
        delay(20);

        z.end_point.x = 0;
        z.end_point.y = 0;
        z.end_point.z = 60;
        RotatePoint(&z.end_point, theta, phi, 0);
        RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
        RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

        for (int n = 0; n < 12; n++) {
            RotatePoint(&rect_source[n].start_point, &rect_dis.start_point,
                        theta, phi, (double)0);
            RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta,
                        phi, (double)0);
            printLine3D(&Disbuff, &rect_dis, TFT_WHITE);
        }
        // Disbuff.fillRect(0,0,160,80,BLACK);
        printLine3D(&Disbuff, &x, TFT_RED);
        printLine3D(&Disbuff, &y, TFT_GREEN);
        printLine3D(&Disbuff, &z, TFT_BLUE);
        /*
        Disbuff.setTextColor(TFT_WHITE);
        Disbuff.setTextSize(1);
        Disbuff.fillRect(0,0,52,18,Disbuff.color565(20,20,20));
        Disbuff.drawString("MPU6886",5,5,1);
        */
        Displaybuff();
        last_theta = theta;
        last_phi   = phi;

        M5.update();
        checkAXPPress();
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);
}

SemaphoreHandle_t xSemaphore = NULL;
SemaphoreHandle_t start_dis  = NULL;
SemaphoreHandle_t start_fft  = NULL;
int8_t i2s_readraw_buff[2048];
uint8_t fft_dis_buff[241][128] = {0};
uint16_t posData               = 160;

void MicRecordfft(void *arg) {
    int16_t *buffptr;
    size_t bytesread;
    uint16_t count_n = 0;
    float adc_data;
    double data = 0;
    uint16_t ydata;

    while (1) {
        xSemaphoreTake(start_fft, portMAX_DELAY);
        xSemaphoreGive(start_fft);
        fft_config_t *real_fft_plan =
            fft_init(1024, FFT_REAL, FFT_FORWARD, NULL, NULL);
        i2s_read(I2S_NUM_0, (char *)i2s_readraw_buff, 2048, &bytesread,
                 (100 / portTICK_RATE_MS));
        buffptr = (int16_t *)i2s_readraw_buff;

        for (count_n = 0; count_n < real_fft_plan->size; count_n++) {
            adc_data =
                (float)map(buffptr[count_n], INT16_MIN, INT16_MAX, -2000, 2000);
            real_fft_plan->input[count_n] = adc_data;
        }
        fft_execute(real_fft_plan);

        xSemaphoreTake(xSemaphore, 100 / portTICK_RATE_MS);
        for (count_n = 1; count_n < real_fft_plan->size / 4; count_n++) {
            data = sqrt(real_fft_plan->output[2 * count_n] *
                            real_fft_plan->output[2 * count_n] +
                        real_fft_plan->output[2 * count_n + 1] *
                            real_fft_plan->output[2 * count_n + 1]);
            if ((count_n - 1) < 128) {
                data  = (data > 2000) ? 2000 : data;
                ydata = map(data, 0, 2000, 0, 255);
                fft_dis_buff[posData][128 - count_n] = ydata;
            }
        }

        posData++;
        if (posData >= 241) {
            posData = 0;
        }
        xSemaphoreGive(xSemaphore);
        fft_destroy(real_fft_plan);
    }
}

void Drawdisplay(void *arg) {
    uint16_t count_x = 0, count_y = 0;
    uint16_t colorPos;
    while (1) {
        xSemaphoreTake(start_dis, portMAX_DELAY);
        xSemaphoreGive(start_dis);
        xSemaphoreTake(xSemaphore, 500 / portTICK_RATE_MS);
        for (count_y = 0; count_y < 128; count_y++) {
            for (count_x = 0; count_x < 240; count_x++) {
                if ((count_x + (posData % 240)) > 240) {
                    colorPos =
                        fft_dis_buff[count_x + (posData % 240) - 240][count_y];
                } else {
                    colorPos = fft_dis_buff[count_x + (posData % 240)][count_y];
                }

                Disbuff.drawPixel(
                    count_x, count_y,
                    Disbuff.color565(ImageData[colorPos * 3 + 0],
                                     ImageData[colorPos * 3 + 1],
                                     ImageData[colorPos * 3 + 2]));
                /*
                disbuff[ count_y * 160 + count_x ].r =  ImageData[ colorPos * 3
                + 0 ]; disbuff[ count_y * 160 + count_x ].g =  ImageData[
                colorPos * 3 + 1 ]; disbuff[ count_y * 160 + count_x ].b =
                ImageData[ colorPos * 3 + 2 ];
                */
            }
        }
        xSemaphoreGive(xSemaphore);
        /*
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);
        Disbuff.fillRect(0,0,70,18,Disbuff.color565(20,20,20));
        Disbuff.drawString("MicroPhone",5,5,1);
        */
        Disbuff.pushSprite(0, 0);
    }
}

TaskHandle_t xhandle_display = NULL;
TaskHandle_t xhandle_fft     = NULL;

void DisplayMicro() {
    Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
    Disbuff.pushSprite(0, 0);

    xSemaphoreGive(start_dis);
    xSemaphoreGive(start_fft);
    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        xSemaphoreGive(start_dis);
        xSemaphoreGive(start_fft);
        M5.update();
        // delay(100);
        xSemaphoreTake(start_dis, portMAX_DELAY);
        xSemaphoreTake(start_fft, portMAX_DELAY);
    }
    // xSemaphoreTake( start_dis , portMAX_DELAY  );
    // xSemaphoreTake( start_fft , portMAX_DELAY  );

    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
}

#define PIN_CLK  0
#define PIN_DATA 34

bool InitI2SMicroPhone() {
    esp_err_t err           = ESP_OK;
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample =
            I2S_BITS_PER_SAMPLE_16BIT,  // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
        .communication_format =
            I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else                                   // 设置通讯格式
        .communication_format = I2S_COMM_FORMAT_I2S,
#endif
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count    = 2,
        .dma_buf_len      = 128,
    };

    i2s_pin_config_t pin_config;
#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif
    pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num    = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num  = PIN_DATA;

    err += i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    err += i2s_set_pin(I2S_NUM_0, &pin_config);
    err += i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT,
                       I2S_CHANNEL_MONO);
    // i2s_set_clk(0)

    if (err != ESP_OK) {
        return false;
    } else {
        return true;
    }
}

void DisplayRTC() {
    Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(0, 0, 0));
    // Displaybuff();
    M5.Rtc.GetBm8563Time();
    RTC_TimeTypeDef time;
    M5.Rtc.GetTime(&time);

    Disbuff.setTextSize(4);
    Disbuff.setCursor(6, 25);
    Disbuff.setTextColor(TFT_WHITE);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(0, 0, 0));
        M5.Rtc.GetTime(&time);
        Disbuff.setTextSize(4);
        Disbuff.setTextColor(TFT_WHITE);
        Disbuff.setCursor(25, 50);
        Disbuff.printf("%02d:%02d:%02d", time.Hours, time.Minutes,
                       time.Seconds);
        Disbuff.fillRect(0, 0, 240, 25, Disbuff.color565(20, 20, 20));
        Disbuff.setTextSize(2);
        Disbuff.drawString("BMP8563 RTC Time", 26, 5, 1);
        Displaybuff();
        M5.update();
        checkAXPPress();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);
}

static esp_pm_lock_handle_t rmt_freq_lock;
#define RMT_TX_CHANNEL  RMT_CHANNEL_0
#define RMT_TX_GPIO_NUM GPIO_NUM_9
#define RMT_CLK_DIV     (1)  // 80000000 / 1(HZ)

rmt_item32_t *tx_buffer = NULL;

void ir_tx_callback(rmt_channel_t channel, void *arg) {
    // static BaseType_t xHigherPriorityTaskWoken = false;
    if (channel == RMT_TX_CHANNEL) {
        esp_pm_lock_release(rmt_freq_lock);
        // xHigherPriorityTaskWoken = pdFALSE;
        // xSemaphoreGiveFromISR( irTxSem, &xHigherPriorityTaskWoken );
        free(tx_buffer);
    }
}

bool InitIRTx() {
    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel  = RMT_TX_CHANNEL;
    rmt_tx.gpio_num = RMT_TX_GPIO_NUM;

    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div       = RMT_CLK_DIV;

    rmt_tx.tx_config.loop_en              = false;
    rmt_tx.tx_config.carrier_duty_percent = 50;
    rmt_tx.tx_config.carrier_freq_hz      = 38000;
    rmt_tx.tx_config.carrier_level        = RMT_CARRIER_LEVEL_LOW;
    rmt_tx.tx_config.carrier_en           = true;
    rmt_tx.tx_config.idle_level           = RMT_IDLE_LEVEL_HIGH;
    rmt_tx.tx_config.idle_output_en       = true;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);

    return true;
}

bool ir_uart_tx(const uint8_t *src, uint8_t len, bool wait_tx_done) {
    /*
    if(src == NULL || recvFlag == true) {
    return false;
    }
    */
    if (src == NULL) {
        return false;
    }

#if BAUD == 4800
    const rmt_item32_t bit0 = {{{8421, 1, 8421, 1}}};  // Logical 0
    const rmt_item32_t bit1 = {{{8421, 0, 8421, 0}}};  // Logical 1
#else
    const rmt_item32_t bit0 = {{{16842, 1, 16842, 1}}};  // Logical 0
    const rmt_item32_t bit1 = {{{16842, 0, 16842, 0}}};  // Logical 1
#endif

    uint8_t *psrc = (uint8_t *)src;
    // xSemaphoreTake(irTxSem, portMAX_DELAY);

    tx_buffer = (rmt_item32_t *)malloc(sizeof(rmt_item32_t) * 10 * len);
    if (tx_buffer == NULL) {
        return false;
    }

    rmt_item32_t *pdest = tx_buffer;

    for (uint8_t ptr = 0; ptr < len; ptr++) {
        pdest->val = bit0.val;
        pdest++;
        for (int i = 0; i < 8; i++) {
            if (*psrc & (0x1 << i)) {
                pdest->val = bit1.val;
            } else {
                pdest->val = bit0.val;
            }
            pdest++;
        }
        pdest->val = bit1.val;
        pdest++;
        psrc++;
    }

    esp_pm_lock_acquire(rmt_freq_lock);
    rmt_write_items(RMT_TX_CHANNEL, tx_buffer, 10 * len, true);
    free(tx_buffer);
    return true;
}

#define SERVICE_UUID           "1bc68b2a-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_RX_UUID "1bc68da0-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_TX_UUID "1bc68efe-f3e3-11e9-81b4-2a2ae2dbcce4"

BLEServer *pServer   = NULL;
BLEService *pService = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected    = false;
bool oldDeviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer) {
        deviceConnected = false;
    }
};

uint8_t *data = new uint8_t[128];

class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        data = pCharacteristic->getData();
    }
};

bool InitBLEServer() {
    uint64_t chipid = ESP.getEfuseMac();
    String blename  = "M5-" + String((uint32_t)(chipid >> 32), HEX);

    BLEDevice::init(blename.c_str());
    // BLEDevice::setPower(ESP_PWR_LVL_N12);
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new MyServerCallbacks());
    pService          = pServer->createService(SERVICE_UUID);
    pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_RX_UUID, BLECharacteristic::PROPERTY_NOTIFY);

    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_TX_UUID, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    return true;
}

bool checkAXP192() {
    float VBat = M5.Axp.GetBatVoltage();

    while (VBat < 3.2) {
        VBat = M5.Axp.GetBatVoltage();
        ErrorDialog(0x22, "Bat Vol error");
    }

    return true;
}

uint8_t crc8(uint8_t data, uint8_t *buff, uint32_t length) {
    uint8_t bit;         // bit mask
    uint8_t crc = 0xFF;  // calculated checksum
    uint8_t byteCtr;     // byte counter
    for (byteCtr = 0; byteCtr < length; byteCtr++) {
        crc ^= (buff[byteCtr]);
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ data;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

int getTempAndHum(float *temp, float *hum) {
    static float _tempSave, _HUmSave;
    uint8_t databuff[6];
    Wire.beginTransmission(0x44);
    Wire.write(0x2c);
    Wire.write(0x06);
    Wire.endTransmission();
    Wire.requestFrom(0x44, 6);

    for (int i = 0; i < 6; i++) {
        databuff[i] = Wire.read();
        Serial.printf("%02X ", databuff[i]);
    }
    Serial.println("--");

    if (crc8(0x31, &databuff[0], 2) == databuff[2]) {
        uint16_t tempraw = databuff[0] << 8 | databuff[1];
        *temp            = -45 + 175 * (float(tempraw) / 65535.0);
        _tempSave        = *temp;
        Serial.printf("Temp:%.2f ", *temp);
    } else {
        *temp = _tempSave;
    }

    if (crc8(0x31, &databuff[3], 2) == databuff[5]) {
        uint16_t Humraw = databuff[3] << 8 | databuff[4];
        *hum            = 100 * (float(Humraw) / 65535.0);
        _HUmSave        = *hum;
        Serial.printf("Hum:%.2f \n", *hum);
    } else {
        *hum = _HUmSave;
    }
    return 0;
}

void DisIRSend() {
    uint8_t senddata[20] = {0};
    memset(senddata, 0x00, sizeof(uint8_t) * 20);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        Disbuff.fillRect(0, 0, 240, 135, TFT_BLACK);
        Disbuff.setSwapBytes(true);
        Disbuff.pushImage(180, 16, 48, 48, (uint16_t *)icon_ir);
        Disbuff.setTextColor(Disbuff.color565(180, 180, 180));
        Disbuff.setTextSize(3);
        Disbuff.setCursor(12, 20);
        Disbuff.printf("IR Send ");

        Disbuff.setTextSize(5);
        Disbuff.setCursor(12, 75);
        if (senddata[0] % 4 == 0) {
            Disbuff.printf("0x%02X>  ", senddata[0]);
        } else if (senddata[0] % 4 == 1) {
            Disbuff.printf("0x%02X>>", senddata[0]);
        } else if (senddata[0] % 4 == 2) {
            Disbuff.printf("0x%02X >>", senddata[0]);
        } else if (senddata[0] % 4 == 3) {
            Disbuff.printf("0x%02X  >", senddata[0]);
        }

        Displaybuff();
        senddata[1]++;
        if (senddata[1] > 3) {
            senddata[1] = 0;
            senddata[0]++;
            ir_uart_tx(senddata, 20, true);
        }
        checkAXPPress();
        M5.update();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);
}

void DisPlayBLESend() {
    uint8_t senddata[2] = {0};

    pService->start();
    pServer->getAdvertising()->start();

    uint64_t chipid = ESP.getEfuseMac();
    String blename  = "M5-" + String((uint32_t)(chipid >> 32), HEX);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        Disbuff.fillRect(0, 0, 240, 135, TFT_BLACK);
        if (deviceConnected) {
            Disbuff.pushImage(180, 16, 48, 48, (uint16_t *)icon_ble);
            Disbuff.setTextColor(Disbuff.color565(180, 180, 180));
            Disbuff.setTextSize(3);
            Disbuff.setCursor(12, 20);
            // Disbuff.printf("BLE connect!\n");
            Disbuff.printf("BLE Send\n");
            Disbuff.setTextSize(5);
            Disbuff.setCursor(12, 75);
            if (senddata[0] % 4 == 0) {
                Disbuff.printf("0x%02X>  ", senddata[0]);
            } else if (senddata[0] % 4 == 1) {
                Disbuff.printf("0x%02X>>", senddata[0]);
            } else if (senddata[0] % 4 == 2) {
                Disbuff.printf("0x%02X >>", senddata[0]);
            } else if (senddata[0] % 4 == 3) {
                Disbuff.printf("0x%02X  >", senddata[0]);
            }

            senddata[1]++;
            if (senddata[1] > 3) {
                senddata[1] = 0;
                senddata[0]++;
                pTxCharacteristic->setValue(senddata, 1);
                pTxCharacteristic->notify();
            }
        } else {
            Disbuff.setTextSize(2);
            Disbuff.setCursor(12, 20);
            Disbuff.setTextColor(TFT_RED);
            Disbuff.printf("BLE disconnect\n");
            Disbuff.setCursor(12, 45);
            Disbuff.setTextColor(Disbuff.color565(18, 150, 219));

            Disbuff.printf(String("Name:" + blename + "\n").c_str());
            Disbuff.setCursor(12, 70);
            Disbuff.printf("UUID:1bc68b2a\n");
            Disbuff.pushImage(180, 16, 48, 48, (uint16_t *)icon_ble_disconnect);
        }
        Displaybuff();

        M5.update();
        delay(100);
        checkAXPPress();
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);
    pService->stop();
    pServer->getAdvertising()->stop();
}

void DisplayGroveSHT30() {
    float tempdata, humdata;
    uint8_t count = 0;

    Wire.begin(32, 33);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        Disbuff.fillRect(0, 0, 240, 135, TFT_BLACK);
        Disbuff.setSwapBytes(true);
        Disbuff.pushImage(180, 16, 48, 48, (uint16_t *)icon_ir);
        Disbuff.setTextColor(Disbuff.color565(180, 180, 180));
        Disbuff.setTextSize(3);
        Disbuff.setCursor(12, 20);
        Disbuff.printf("IR Send ");

        if (count >= 10) {
            count = 0;
            getTempAndHum(&tempdata, &humdata);

            Disbuff.setCursor(12, 50);
            Disbuff.printf("%.2f", tempdata);
            Disbuff.setCursor(12, 80);
            Disbuff.printf("%.2f", humdata);
            Disbuff.pushSprite(0, 0);
        }
        checkAXPPress();
        M5.update();
        delay(100);
        count++;
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);
}
void DisplayTestMode() {
    float tempdata, humdata;
    uint8_t count = 10, count_u = 0, count_t = 0;

    i2s_pin_config_t pin_config;
    pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num    = 33;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num  = PIN_DATA;
    i2s_set_pin(I2S_NUM_0, &pin_config);

    i2s_driver_uninstall(I2S_NUM_0);

    gpio_reset_pin(GPIO_NUM_0);
    gpio_reset_pin(GPIO_NUM_26);

    pinMode(26, OUTPUT);
    pinMode(25, INPUT_PULLDOWN);
    pinMode(36, INPUT_PULLDOWN);
    pinMode(0, OUTPUT);

    digitalWrite(0, 0);
    digitalWrite(26, 0);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed())) {
        Disbuff.fillRect(0, 0, 240, 135, TFT_BLACK);
        Disbuff.setSwapBytes(true);
        Disbuff.setTextColor(Disbuff.color565(180, 180, 180));
        Disbuff.setTextSize(3);

        Disbuff.setCursor(12, 7);
        if (M5.Axp.GetBatVoltage() > 3.2) {
            Disbuff.setTextColor(TFT_GREEN);
        } else {
            Disbuff.setTextColor(TFT_RED);
        }
        Disbuff.printf("%.2f", M5.Axp.GetBatVoltage());
        Disbuff.setCursor(12, 37);
        if (M5.Axp.GetVinVoltage() > 4.6) {
            Disbuff.setTextColor(TFT_GREEN);
        } else {
            Disbuff.setTextColor(TFT_RED);
        }
        Disbuff.printf("%.2f", M5.Axp.GetVinVoltage());

        digitalWrite(0, 0);
        digitalWrite(26, 0);
        count_u = 0;
        count_t = 0;

        for (int i = 0; i < 10; i++) {
            digitalWrite(0, i % 2);
            delay(10);
            // pin36_adc = analogRead(36);
            if ((digitalRead(36) == HIGH) && (i % 2 == 1)) {
                count_u++;
            }
            if ((digitalRead(25) == HIGH) && (i % 2 == 1)) {
                count_t++;
            }
        }

        Disbuff.setCursor(110, 7);
        if (count_u >= 5) {
            Disbuff.setTextColor(TFT_GREEN);
            Disbuff.printf("%d G0", count_u);
        } else {
            Disbuff.setTextColor(TFT_RED);
            Disbuff.printf("%d G0", count_u);
        }
        Disbuff.setTextColor(TFT_WHITE);

        Disbuff.setCursor(110, 37);
        if (count_t >= 5) {
            Disbuff.setTextColor(TFT_GREEN);
            Disbuff.printf("%d G25", count_t);
        } else {
            Disbuff.setTextColor(TFT_RED);
            Disbuff.printf("%d G25", count_t);
        }
        Disbuff.setTextColor(TFT_WHITE);

        digitalWrite(0, 0);
        digitalWrite(26, 0);
        count_u = 0;

        for (int i = 0; i < 10; i++) {
            digitalWrite(26, i % 2);
            delay(10);
            // pin36_adc = analogRead(36);
            if ((digitalRead(36) == HIGH) && (i % 2 == 1)) {
                count_u++;
            }
        }

        Disbuff.setCursor(110, 67);
        if (count_u >= 5) {
            Disbuff.setTextColor(TFT_GREEN);
            Disbuff.printf("%d G26", count_u);
        } else {
            Disbuff.setTextColor(TFT_RED);
            Disbuff.printf("%d G26", count_u);
        }
        Disbuff.setTextColor(TFT_WHITE);

        digitalWrite(0, 0);
        digitalWrite(26, 0);
        // Serial.printf("G36 Vol:%d\n",analogRead(36));

        if (count >= 10) {
            count = 0;
            getTempAndHum(&tempdata, &humdata);
        }

        Disbuff.setTextColor(TFT_WHITE);
        Disbuff.setCursor(12, 67);
        Disbuff.printf("%.1f", tempdata);
        Disbuff.setCursor(12, 97);
        Disbuff.printf("%.1f", humdata);

        Disbuff.pushSprite(0, 0);

        checkAXPPress();
        M5.update();
        delay(10);
        count++;
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed())) {
        M5.update();
        checkAXPPress();
        M5.Beep.tone(4000);
        delay(10);
    }
    delay(50);
    M5.Beep.mute();
    Disbuff.setTextColor(TFT_WHITE);

    InitI2SMicroPhone();
}

void ColorBar() {
    float color_r, color_g, color_b;

    color_r = 0;
    color_g = 0;
    color_b = 255;

    for (int i = 0; i < 384; i = i + 4) {
        if (i < 128) {
            color_r = i * 2;
            color_g = 0;
            color_b = 255 - (i * 2);
        } else if ((i >= 128) && (i < 256)) {
            color_r = 255 - ((i - 128) * 2);
            color_g = (i - 128) * 2;
            color_b = 0;
        } else if ((i >= 256) && (i < 384)) {
            color_r = 0;
            color_g = 255 - ((i - 256) * 2);
            ;
            color_b = (i - 256) * 2;
            ;
        }
        Disbuff.fillRect(0, 0, 240, 135,
                         Disbuff.color565(color_r, color_g, color_b));
        Displaybuff();
    }

    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                color_r = 0;
                color_g = 0;
                color_b = 0;
                break;
            case 1:
                color_r = 255;
                color_g = 0;
                color_b = 0;
                break;
            case 2:
                color_r = 0;
                color_g = 255;
                color_b = 0;
                break;
            case 3:
                color_r = 0;
                color_g = 0;
                color_b = 255;
                break;
        }
        for (int n = 0; n < 240; n++) {
            color_r = (color_r < 255) ? color_r + 1.0625 : 255U;
            color_g = (color_g < 255) ? color_g + 1.0625 : 255U;
            color_b = (color_b < 255) ? color_b + 1.0625 : 255U;
            Disbuff.drawLine(n, i * 33.75, n, (i + 1) * 33.75,
                             Disbuff.color565(color_r, color_g, color_b));
        }
    }
    Displaybuff();
    delay(500);

    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                color_r = 255;
                color_g = 255;
                color_b = 255;
                break;
            case 1:
                color_r = 255;
                color_g = 0;
                color_b = 0;
                break;
            case 2:
                color_r = 0;
                color_g = 255;
                color_b = 0;
                break;
            case 3:
                color_r = 0;
                color_g = 0;
                color_b = 255;
                break;
        }
        for (int n = 0; n < 240; n++) {
            color_r = (color_r > 2) ? color_r - 1.0625 : 0U;
            color_g = (color_g > 2) ? color_g - 1.0625 : 0U;
            color_b = (color_b > 2) ? color_b - 1.0625 : 0U;
            Disbuff.drawLine(239 - n, i * 33.75, 239 - n, (i + 1) * 33.75,
                             Disbuff.color565(color_r, color_g, color_b));
        }
    }
    Displaybuff();
    delay(500);
}

uint8_t addrcheckbuff[3] = {
    0x34,  //
    0x51,  //
    0x68   //
};

int checkI2CAddr() {
    uint8_t faild_count = 0;

    do {
        faild_count = 0;
        for (int i = 0; i < sizeof(addrcheckbuff); i++) {
            Wire1.beginTransmission(addrcheckbuff[i]);
            if (Wire1.endTransmission() == ESP_OK) {
                Serial.printf("find %02X addr successful\r\n",
                              addrcheckbuff[i]);
            } else {
                Serial.printf("find %02X addr faild\r\n", addrcheckbuff[i]);
                char strbuff[128];
                sprintf(strbuff, "i2c %02X fail", addrcheckbuff[i]);
                ErrorDialog(addrcheckbuff[i], (const char *)strbuff);
                faild_count++;
            }
        }
    } while (faild_count != 0);

    return 0;
}

void setup() {
    M5.begin();

    Wire.begin(32, 33);

    M5.Lcd.setRotation(3);

    Disbuff.createSprite(240, 135);
    Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(10, 10, 10));
    Disbuff.pushSprite(0, 0);
    delay(500);

    M5.update();
    if (M5.BtnB.isPressed()) {
        M5.Beep.tone(4000);
        delay(100);
        M5.Beep.mute();
        TestMode = true;

        while (M5.BtnB.isPressed()) {
            M5.update();
            delay(10);
        }
    }
    M5.Axp.ScreenBreath(80);

    ColorBar();

    checkI2CAddr();
    checkAXP192();

    M5.Imu.Init();
    InitI2SMicroPhone();
    InitIRTx();
    InitBLEServer();

    pinMode(10, OUTPUT);
    timerSemaphore = xSemaphoreCreateBinary();
    timer          = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 50000, true);
    timerAlarmEnable(timer);

    xSemaphore = xSemaphoreCreateMutex();
    start_dis  = xSemaphoreCreateMutex();
    start_fft  = xSemaphoreCreateMutex();

    xSemaphoreTake(start_dis, portMAX_DELAY);
    xSemaphoreTake(start_fft, portMAX_DELAY);

    xTaskCreate(Drawdisplay, "Drawdisplay", 1024 * 2, (void *)0, 4,
                &xhandle_display);
    xTaskCreate(MicRecordfft, "MicRecordfft", 1024 * 2, (void *)0, 5,
                &xhandle_fft);

    Disbuff.pushSprite(0, 0);
}
bool beepstate = false;

void loop() {
    MPU6886Test();
    DisplayRTC();
    DisplayMicro();
    DisIRSend();

    if (TestMode) {
        DisplayTestMode();
    }

    DisPlayBLESend();
    M5.update();
    delay(50);
}
