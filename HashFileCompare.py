/*
author:zyr
function:自行配置路径后，可以快速的通过哈希值对恢复的文件进行比对
notice:none
*/
import glob
import hashlib
from colorama import init

init(autoreset=True)
md5_map = {
    'EEC87931B03E5A4A4EF8FD51109A1227': '1a.html',
    'A80EE062AED8279304FAAE8F20F6D48E': '1b.html',
    'FE7E7AC67709F2D9C2483AA98C681B99': '1b.jpg',
    '045798407B927321326A547704E67831': '1c.html',
    '616A6BBE915C3DBF51014FD76F55B0E3': '1c.txt',
    '8A1AC28FE7BD144B8230B38B284A3827': '1d1.html',
    '80E6B9221EF308FF1639FD19DF036E34': '1d2.html',
    '0E52E75029E99CD2E9DCD0AF271CF4A2': '2a.doc',
    '4A22F04B097920D11FFF4E192E0667A4': '2b.doc',
    '03D1DEFF4774C932358D3580A3BBAE66': '2c.xls',
    '8D2A9A284E078805ADA47DB191F35244': '2d.doc',
    '4EFC6C572683878EFD8F3404DDADED7B': '2d.jpg',
    'D7FF92B8CC1C89C46A78288B9C673152': '2e.doc',
    '5A12EF9DBA88A186EF18A5D349B28E37': '2e.txt',
    'DAF4205574ABD6919B10CA8BE92D17A3': '3a.jpg',
    'B070BEAE1606F67A342BC5F78C29C743': '3b.jpg',
    '2FAE8770CC013D22E9EA1C070F2F509B': '3c.jpg',
    '7B07320709E0CAA947663F5DF3A0A390': '3d.jpg',
    'F800A46E18FAFD309825C5EE84A654A2': '3d.txt',
    '109284CC5ABDDC83879A29785795FD75': '3e.doc',
    '2320FE9C41EADDB864A56C2DDC4DD186': '3e.jpg',
    'F8C51E0688796B5D616F0E5D4A94D104': '3f.jpg',
    '7CCE072E518FD72484C97ADB1B4BE08E': '3g1.jpg',
    'C0DA37B3F1A07AF790E6E9171CEDC4D2': '3g2.jpg',
    'DB89684C177168036E274140ECF766A1': '3h1.jpg',
    '0915313E99AF0F6BF13BC06BCD003113': '3h2.jpg',
    'DB32B271506B2F4974791957627C61CC': '3i.jpg',
    '1A5A843000EF617AF93A9CAD645E3CDF': '3j.jpg',
    'EBABDE39BA44D38888DD82606980498A': '4a.zip',
    '643D159339BD2C9E7604E6FAC8E7FACC': '4b.html',
    '9A4C2D3A9BD203EB39C9F954A3C997E4': '4b.zip'}

if __name__ == "__main__":
    matched_total = 0
    for file in glob.glob(r"E:\1_Temp\challenges\MyOutput\*.jpg"):
        with open(file, "rb") as f:
            file_bytes = f.read()  # read file as bytes
            readable_hash = hashlib.md5(file_bytes).hexdigest().upper()
            print(file, readable_hash)
            if readable_hash in md5_map:
                matched_total += 1
                print('\033[1;30;47m' + str(file) + " matches " + str(md5_map[readable_hash]))

    if matched_total == 0:
        print('\033[1;31;40m'"No aim file found.")
    else:
        print('\033[1;37;46m'f"Matched {matched_total} files.")

# 直接暴力匹配：   2d,3b,1b,3c,3g2,3i
# 使用字节跳转：   3a,2d,3b,1b,3g2,3i
# 少了3c，多了3a
