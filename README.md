# ESP32-DHT22 🌡️🌍
![Hits](https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2Fmehrdadmb2%2FESP32-DHT22&count_bg=%2379C83D&title_bg=%23555555&icon=github.svg&icon_color=%23E7E7E7&title=visits&edge_flat=false)
![GitHub license](https://img.shields.io/github/license/mehrdadmb2/ESP32-DHT22)
![GitHub stars](https://img.shields.io/github/stars/mehrdadmb2/ESP32-DHT22?style=social)
![GitHub forks](https://img.shields.io/github/forks/mehrdadmb2/ESP32-DHT22?style=social)
![GitHub issues](https://img.shields.io/github/issues/mehrdadmb2/ESP32-DHT22)

Welcome to the ESP32-DHT22 project! This repository contains code and resources to monitor **temperature** and **humidity** data using an ESP32 microcontroller and a DHT22 sensor. 📊✨ Perfect for IoT enthusiasts, students, and hobbyists looking to build and customize their own environment monitoring system.

---

## 📜 Table of Contents
- [Features](#features)
- [Setup](#setup)
- [Installation](#installation)
- [Usage](#usage)
- [Demo](#demo)
- [Contributing](#contributing)
- [License](#license)

---

## 🌟 Features
- **Real-time Data**: Capture live temperature and humidity data from the DHT22 sensor.
- **Wi-Fi Connectivity**: Easily connect your ESP32 to the internet and view data from any device.
- **Data Visualization**: OLED display support to show readings on the ESP32 itself.
- **Easy Setup**: Minimal wiring and simple code to get started quickly.

---

## 🛠️ Setup

### Requirements
- **ESP32 Board**: Any model compatible with Arduino IDE.
- **DHT22 Sensor**: Temperature and humidity sensor.
- **OLED Display (Optional)**: For displaying real-time data.

### Wiring
1. Connect the **VCC** pin on DHT22 to **3.3V** on the ESP32.
2. Connect the **GND** pin on DHT22 to **GND** on the ESP32.
3. Connect the **Data** pin on DHT22 to any **GPIO pin** (update the code accordingly).

---

## 🚀 Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/mehrdadmb2/ESP32-DHT22.git
   ```
2. **Install Required Libraries**: Ensure the following libraries are installed in Arduino IDE:
   - **DHT sensor library**
   - **Adafruit Unified Sensor**
   - **WiFi** (for internet connectivity)
   - **Adafruit_GFX** and **Adafruit_SSD1306** (for OLED)

3. **Upload Code**: Open the `.ino` file in Arduino IDE, select the correct board and port, then upload the code.

---

## 📈 Usage

1. **View Data Locally**: With the OLED screen connected, data will display in real-time on the ESP32.
2. **Monitor via Wi-Fi**: Connect your ESP32 to a Wi-Fi network to transmit data for remote monitoring.

### Customization
- Change the **Wi-Fi credentials** and **GPIO pins** in the code to match your setup.
- Modify update intervals or data display formats as per project needs.

---

## 🎥 Demo
![ESP32 DHT22 Demo](https://user-images.githubusercontent.com/XXXXXX/DEMO.gif)

*Example setup of the ESP32-DHT22 with real-time data display on OLED.* *(Replace with your image/GIF)*

---

## 🤝 Contributing
Feel free to contribute to this project by opening issues, submitting pull requests, or suggesting new features. Check out the [CONTRIBUTING.md](CONTRIBUTING.md) for more guidelines.

---

## 📝 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

--- 

Feel free to replace the demo image or GIF link with an actual file if you have one. Let me know if you’d like any other tweaks!
