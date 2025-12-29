# QT_Enhanced_Image_Processor_1229_Mouse_Event_Handling

## 功能說明 (Features)

這是一個基於Qt5的增強型圖像處理器，支援滑鼠事件處理和圖像編輯功能。

### 主要功能

1. **圖片載入**
   - 支援PNG、JPG、JPEG等常見圖片格式
   - 使用「開啟」按鈕選擇圖片

2. **放大模式 (Zoom Mode)**
   - 啟用「放大模式」複選框後，可以在圖片上拖曳選取矩形區域
   - 使用橡皮筋視覺效果顯示選取區域
   - 自訂放大倍率 (1x - 10x)
   - 選取完成後自動彈出新視窗顯示放大的圖片

3. **圖片編輯視窗**
   放大後的新視窗提供以下功能：
   - **另存新檔**: 將編輯後的圖片儲存為PNG、JPEG等格式
   - **畫筆模式**: 啟用後可以在圖片上繪製
     - 可調整筆刷大小 (1-50像素)
     - 可選擇筆刷顏色
     - 使用滑鼠拖曳進行繪圖

4. **滑鼠追蹤**
   - 即時顯示滑鼠位置座標
   - 顯示當前像素的灰階值

## 建置說明 (Build Instructions)

### 需求
- Qt5 (qtbase5-dev)
- qmake
- C++17編譯器

### 編譯步驟
```bash
cd QT_Enhanced_Image_Processor_1229_Mouse_Event_Handling
qmake
make
```

### 執行
```bash
./QT_Enhanced_Image_Processor_1229_Mouse_Event_Handling
```

## 使用方法 (Usage)

1. 啟動程式後，點擊「開啟」按鈕載入圖片
2. 勾選「放大模式」複選框
3. 設定想要的放大倍率
4. 在圖片上拖曳滑鼠選取要放大的區域
5. 放開滑鼠後會彈出新視窗顯示放大的圖片
6. 在新視窗中：
   - 勾選「畫筆模式」可以開始繪圖
   - 調整筆刷大小和顏色
   - 點擊「另存新檔」儲存編輯後的圖片

## 檔案結構 (File Structure)

- `main.cpp` - 程式進入點
- `mouseevent.h/cpp` - 主視窗類別，處理圖片載入和區域選取
- `zoomwindow.h/cpp` - 放大/編輯視窗類別，提供繪圖和儲存功能
- `QT_Enhanced_Image_Processor_1229_Mouse_Event_Handling.pro` - Qt專案檔
