import requests
import base64
import hashlib
def crypt(data,key) :
    s = [0] * 256
    for i in range(256) :
        s[i] = i
    print(s)
    j = 0
    for i in range(256) :
        j = (j + s[i] + key[i % len(key)]) % 256
        print(j)
        s[i], s[j] = s[j], s[i]
    i = 0
    j = 0
    res = ""
    for c in data :
        i = (i + 1) % 256
        j = (j + s[i]) % 256
        s[i], s[j] = s[j], s[i]
        res = res + chr(c ^ s[(s[i] + s[j]) % 256])
    return res

def tdecode(data ,key) :
    data = base64.b64decode(data)
    salt = data[:16]
    return crypt(data[16:] ,hashlib.sha1(bytes(key,encoding = "utf8") + salt).digest())

if __name__ =='__main__':
    key = "secrets" 			#填写key
    data = "密文"			#填写密文
    print(tdecode(data,key))
