import socket
import sys

print("=== STM32 TCP Connection Test ===")
print(f"Target: 192.168.0.100:2001")
print("=" * 40)

try:
    # 1. Подключаемся
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(5)
    
    print("Step 1: Connecting to STM32...")
    sock.connect(("192.168.0.100", 2001))
    print("✅ Connection established!")
    print("")
    
    # 2. Получаем приветствие
    print("Step 2: Receiving welcome message...")
    sock.settimeout(2)
    try:
        welcome = sock.recv(1024)
        if welcome:
            print(f"✅ Welcome: {welcome.decode().strip()}")
        else:
            print("⚠ No welcome message")
    except socket.timeout:
        print("⚠ No immediate response (timeout)")
    print("")
    
    # 3. Тест SLCAN команды
    print("Step 3: Testing SLCAN command 'V\\r\\n'...")
    sock.send(b"V\r\n")
    
    try:
        response = sock.recv(1024)
        if response:
            print(f"✅ Response: {response.decode().strip()}")
        else:
            print("⚠ No response")
    except socket.timeout:
        print("⚠ Timeout waiting for response")
    print("")
    
    # 4. Тест команды версии
    print("Step 4: Testing 'version\\r\\n'...")
    sock.send(b"version\r\n")
    
    try:
        response = sock.recv(1024)
        if response:
            print(f"✅ Response: {response.decode().strip()}")
        else:
            print("⚠ No response")
    except socket.timeout:
        print("⚠ Timeout")
    print("")
    
    # 5. Закрываем соединение
    sock.close()
    print("✅ Test completed successfully!")
    
except ConnectionRefusedError:
    print("❌ Connection refused - TCP server not running")
    print("Check if STM32 is powered and Ethernet cable is connected")
except socket.timeout:
    print("❌ Connection timeout - No response from device")
    print("Check IP address and network configuration")
except Exception as e:
    print(f"❌ Unexpected error: {type(e).__name__}: {e}")

print("=" * 40)
input("\nPress Enter to exit...")