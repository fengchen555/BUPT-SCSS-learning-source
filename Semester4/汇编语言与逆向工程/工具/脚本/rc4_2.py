def rc4_decrypt(ciphertext, key):
    key_length = len(key)
    S = list(range(256))

    # 密钥取反操作
    inverted_key = [~byte & 0xFF for byte in key]

    j = 0
    for i in range(256):
        j = (j + S[i] + inverted_key[i % key_length]) % 256
        S[i], S[j] = S[j], S[i]

    i = j = 0
    plaintext = bytearray()

    for byte in ciphertext:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        k = S[(S[i] + S[j]) % 256] ^ byte
        plaintext.append(k)

    return bytes(plaintext)


# 加密文本
ciphertext = b'\x2C\xFC\xFB\x3B\x0D\x73\x6E\xBC\xB9\xA7\x6F\x2F'

# 已知密钥
key = b'secrets'

plaintext = rc4_decrypt(ciphertext, key)

try:
    plaintext_str = plaintext.decode('utf-8')  # 使用utf-8编码解码为字符串
except UnicodeDecodeError:
    plaintext_str = plaintext.decode('latin-1')  # 使用latin-1编码解码为字符串

print("Plaintext: ", plaintext_str)
