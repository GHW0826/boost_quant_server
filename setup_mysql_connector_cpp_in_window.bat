@echo off
REM Cpp mysql connector ZIP 파일 다운로드 스크립트

REM 로그 시작
echo ======================================
echo Mysql Connector Download SCRIPT STARTED
echo ======================================

REM 다운로드할 Boost 버전 설정
SET MYSQL_CONNECTOR_VERSION=9.1.0
SET MYSQL_CONNECTOR_UNDERSCORE=9_1_0

REM 로그: Mysql Connector 버전 출력
echo Step 1: Setting mysql connector version
echo mysql connector version: %MYSQL_CONNECTOR_VERSION%
echo mysql connector version (underscore): %MYSQL_CONNECTOR_UNDERSCORE%
echo.

REM 다운로드 URL 구성

SET MYSQL_CONNECTOR_URL=https://dev.mysql.com/get/Downloads/Connector-C++/mysql-connector-c++-%MYSQL_CONNECTOR_VERSION%-winx64.zip
SET OUTPUT_FILE=mysql_connector_%MYSQL_CONNECTOR_UNDERSCORE%.zip
SET TARGET_FOLDER=mysql_connector_%MYSQL_CONNECTOR_UNDERSCORE%

REM 로그: 다운로드 URL 출력
echo Step 2: Configuring download URL
echo Download URL: %MYSQL_CONNECTOR_URL%
echo Output file: %OUTPUT_FILE%
echo.

REM 다운로드 결과 확인
IF EXIST %OUTPUT_FILE% (
    echo ======================================
    echo Exist Boost Zip File
    echo ======================================
    pause
    exit /b 1
)

REM 다운로드 시작
echo Step 3: Starting download with curl
curl -L -o %OUTPUT_FILE% %MYSQL_CONNECTOR_URL%
echo.

REM 다운로드 결과 확인
IF EXIST %OUTPUT_FILE% (
    echo Step 4: Download completed successfully
    echo File downloaded: %OUTPUT_FILE%
) ELSE (
    echo Step 4: Download failed
    echo Please check the URL or your internet connection.
    echo.
    pause
    exit /b 1
)


REM 압축 해제
echo Step 5: Extracting ZIP file to %TARGET_FOLDER%
IF NOT EXIST %TARGET_FOLDER% (
    mkdir %TARGET_FOLDER%
)
REM 압축 해제를 TARGET_FOLDER로 수행
tar -xf %OUTPUT_FILE% --strip-components=1 -C %TARGET_FOLDER%

REM 압축 해제 확인
IF EXIST %TARGET_FOLDER% (
    echo Extraction completed successfully: %TARGET_FOLDER%
) ELSE (
    echo Extraction failed. Please check the ZIP file and extraction process.
    pause
    exit /b 1
)

pause


