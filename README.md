# Debug 406 实验室 2025 年电赛  
## 逐飞科技 MSPM0G3507 开源工程

本项目为 Debug 406 实验室 2025 年全国大学生电子设计竞赛（电赛）专用，基于逐飞科技 MSPM0G3507 TI 板的开源工程。工程集成了逐飞科技自研的 MSPM0G3507 开源库，适用于 MSPM0G3507 平台的各类外设移植、测试与竞赛开发。

---

## 项目简介

- **适用平台**：MSPM0G3507（TI 逐飞板）
- **开发环境**：MDK 5.37 / VSCode + EIDE
- **开源协议**：GPL v3.0
- **当前版本**：V1.0.0（初始版本）

本工程基于官方 SDK，集成了丰富的外设驱动和组件，适合用于电赛项目的快速开发和功能验证。主程序默认在屏幕上显示“DEBUG 406”和“Dian Sai”，便于调试和展示。

---

## 主要目录结构

- `project/`  
  - `user/` 用户主控代码（如 `main.c`、`isr.c` 等）
  - `code/` 其他代码或文档
- `libraries/`  
  - `zf_device/` 逐飞外设驱动（如屏幕、IMU、无线等）
  - `zf_driver/` 逐飞底层驱动（GPIO、UART、SPI、PWM、ADC等）
  - `zf_common/` 通用工具与基础代码
  - `zf_components/` 逐飞功能组件
  - `sdk/` TI 官方 SDK 及底层库
  - `doc/` 文档与版本说明
- `build/` 编译输出目录
- `.vscode/` VSCode 配置
- `.eide/` EIDE 工程配置

---

## 快速上手

1. 使用 VSCode + EIDE 或 Keil MDK 打开本工程。
2. 连接 MSPM0G3507 板卡，编译并下载程序。
3. 默认主程序会在屏幕上显示“DEBUG 406”和“Dian Sai”，可据此进行后续开发。

---

## 开源协议

本项目采用 GPL v3.0 协议开源，欢迎学习、传播和二次开发。请保留逐飞科技相关版权声明。

---

## 联系与支持

- 逐飞科技淘宝店：[https://seekfree.taobao.com/](https://seekfree.taobao.com/)
- Debug 406 实验室

---

如需补充具体模块说明、外设支持列表或英文版文档，请告知！ 