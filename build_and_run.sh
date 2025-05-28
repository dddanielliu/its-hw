#!/bin/bash

# 車聯網模擬編譯與執行腳本

echo "=== 檢查 Veins 環境 ==="
if [ -z "$VEINS_ROOT" ]; then
    echo "錯誤: VEINS_ROOT 環境變數未設置"
    echo "請設置 VEINS_ROOT 指向 Veins 安裝目錄"
    exit 1
fi

if [ -z "$SUMO_HOME" ]; then
    echo "警告: SUMO_HOME 環境變數未設置"
    echo "請確保 SUMO 已安裝並可執行"
fi

echo "VEINS_ROOT: $VEINS_ROOT"
echo "SUMO_HOME: $SUMO_HOME"

echo ""
echo "=== 生成訊息類別 ==="
# 生成 C++ 訊息類別
if [ -f "MyVeinsApp_m.msg" ]; then
    opp_msgc MyVeinsApp_m.msg
    if [ $? -eq 0 ]; then
        echo "訊息類別生成成功"
    else
        echo "錯誤: 訊息類別生成失敗"
        exit 1
    fi
else
    echo "錯誤: 找不到 MyVeinsApp_m.msg 文件"
    exit 1
fi

echo ""
echo "=== 生成 Makefile ==="
# 自動生成 Makefile
opp_makemake -f --deep -O out -I$VEINS_ROOT/src -L$VEINS_ROOT/out/gcc-release/src -lveins

if [ $? -eq 0 ]; then
    echo "Makefile 生成成功"
else
    echo "錯誤: Makefile 生成失敗"
    exit 1
fi

echo ""
echo "=== 編譯專案 ==="
make MODE=release

if [ $? -eq 0 ]; then
    echo "編譯成功"
else
    echo "錯誤: 編譯失敗"
    exit 1
fi

echo ""
echo "=== 檢查必要文件 ==="
required_files=("net.net.xml" "routes.rou.xml" "net.sumocfg" "omnetpp.ini")
for file in "${required_files[@]}"; do
    if [ ! -f "$file" ]; then
        echo "警告: 找不到 $file"
    else
        echo "✓ $file 存在"
    fi
done

echo ""
echo "=== 啟動模擬 ==="
echo "請選擇運行模式:"
echo "1) GUI 模式 (omnetpp)"
echo "2) 命令行模式 (opp_run)"
echo "3) 除錯模式 (omnetpp with debugger)"
read -p "請輸入選項 (1-3): " choice

case $choice in
    1)
        echo "啟動 GUI 模式..."
        omnetpp
        ;;
    2)
        echo "啟動命令行模式..."
        opp_run -l ./homework -n . -u Cmdenv -c General omnetpp.ini
        ;;
    3)
        echo "啟動除錯模式..."
        omnetpp -d
        ;;
    *)
        echo "無效選項，使用預設 GUI 模式..."
        omnetpp
        ;;
esac

echo ""
echo "=== 執行完成 ==="
