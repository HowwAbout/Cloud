# Cloud Discord Bot

## 🖼 Preview

![log 복사본 3](https://github.com/user-attachments/assets/2d26c76f-3d3d-4a19-b085-9be7f49a7b1a)

##  Table of Contents

- [ Overview](#-overview)
- [ Project Structure](#-project-structure)
- [ Getting Started](#-getting-started)
  - [ Prerequisites](#prerequisites)
  - [ Installation](#installation)
  - [ Usage](#usage)

## 📝 Overview
The Deploy_DiscordBot is a C++ application designed to interact with Discord's API, providing various functionalities such as automated responses, notifications, and integrations with other services.

### Main Purpose
- The primary goal of the project is to create a versatile Discord bot that can automate tasks and enhance user interaction within Discord servers.
- It solves the problem of manual task management and provides a seamless way to integrate external services with Discord.
- The target audience includes Discord server administrators and users looking to enhance their server's functionality.

### Key Features
- Automated responses to user commands.
- Integration with external services like Jenkins for notifications.
- Configuration management for easy setup and customization.

### Core Technology Stack
- Database: N/A
- Other Tools: CMake, Makefile

## 📁 Project Structure
```
Cloud-discord_bot
├── 📁 srcs
│   ├── bot.cpp
│   ├── jenkins_utils.cpp
│   ├── dpp_utils.cpp
│   ├── config.cpp
│   └── main.cpp
├── 📁 include
│   ├── bot.h
│   ├── jenkins_utils.h
│   ├── dpp_utils.h
│   └── config.h
├── 📁 libs
│   ├── 📁 json
│   │   ├── CMakeLists.txt
│   │   ├── include
│   │   ├── single_include
│   │   └── ...
│   └── ...
├── 📁 build
├── 📁 bin
└── Makefile
```

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler (e.g., clang++)
- CMake (version 3.1 or higher)
- Required libraries: dpp, curl

### Installation
```bash
# Clone the repository
git clone https://github.com/42kko/42.git

# Navigate to the project directory
cd 42/Cloud-discord_bot

# Install required packages
make

# Configure environment
# Ensure that a `.env` file exists in the `config/` directory.
# The `.env` file must include the following environment variables:
# - BOT_TOKEN: Your Discord bot token
# - JENKINS_TOKEN: Token for Jenkins API access
# - JENKINS_URL: URL for the Jenkins server
#
# Example:
# BOT_TOKEN=your-discord-bot-token
# JENKINS_TOKEN=your-jenkins-token
# JENKINS_URL=https://your-jenkins-server-url
```

### Usage
```bash
# How to run the bot
./bin/mybot
```
