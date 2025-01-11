@echo off
REM Cpp redis ZIP 파일 다운로드 스크립트

@echo off
:: 클론할 리포지토리 URL
set REPO_URL=https://github.com/cylix/cpp_redis

REM 로그 시작
echo ======================================
echo Cloning repository from %REPO_URL%
echo ======================================

REM cpp redis git clone
git clone %REPO_URL%
if %errorlevel% neq 0 (
    echo Failed to clone repository.
    exit /b 1
)

pause