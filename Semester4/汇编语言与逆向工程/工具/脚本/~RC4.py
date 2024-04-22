def generate_rc4_key(seed_key):
    # 初始化 S 盒
    S = list(range(256))
    j = 0
    key = [ord(c) for c in seed_key]

    # 初始置换
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]

    # 生成 RC4 密钥序列
    i = 0
    j = 0
    while True:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        k = S[(S[i] + S[j]) % 256]
        yield k


# 测试代码
seed_key = "secrets"
rc4_key_generator = generate_rc4_key(seed_key)

# 生成 RC4 密钥序列的前 n 个值
rc4_key_sequence = [next(rc4_key_generator) for _ in range(14)]
str1 = ''
# for i in range(len(rc4_key_sequence)):
#     str1+=chr(256-rc4_key_sequence[i])
# print(str1)
str2= [0x10,0x2C,0x02,0xFC,0xFB,0x3B,0x0D,0x73,0x6E,0xBC,0xB9,0xA7,0x6F,0x2F,]
for i in  range(14):
    str1 += chr((255 - rc4_key_sequence[i])^str2[i])
print(str1)
