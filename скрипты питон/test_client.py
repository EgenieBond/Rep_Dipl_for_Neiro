import socket
import os

print("=== Simple STM32 Network Test ===")
print("")

# Очистим ARP кэш
print("1. Clearing ARP cache...")
os.system("arp -d 192.168.50.100 2>nul")

# Пинг
print("\n2. Pinging STM32 (192.168.50.100)...")
response = os.system("ping -n 2 192.168.50.100")
if response == 0:
    print("✅ Ping SUCCESS")
else:
    print("❌ Ping FAILED")

# TCP тест
print("\n3. Testing TCP port 2001...")
try:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(3)
    sock.connect(("192.168.50.100", 2001))
    print("✅ TCP Connection SUCCESS")
    
    # Пробуем получить данные
    sock.settimeout(1)
    try:
        data = sock.recv(1024)
        if data:
            print(f"   Received: {data.decode('utf-8', errors='ignore')[:50]}")
    except:
        print("   ⚠ No immediate response")
    
    # Отправляем тест
    print("\n4. Sending test command...")
    sock.send(b"V\r\n")
    
    try:
        response = sock.recv(1024)
        if response:
            print(f"✅ Response: {response.decode('utf-8', errors='ignore')}")
        else:
            print("⚠ No response")
    except:
        print("⚠ Timeout")
    
    sock.close()
    
except socket.timeout:
    print("❌ TCP Connection TIMEOUT")
except ConnectionRefusedError:
    print("❌ Connection REFUSED")
except Exception as e:
    print(f"❌ Error: {e}")

print("\n" + "=" * 40)
input("Press Enter to exit...")