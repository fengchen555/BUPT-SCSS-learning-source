import string
import hashlib
import itertools

length = 4  # 长度
func = hashlib.md5  # Hash 方法
char = string.ascii_letters + string.digits  # 字符集

result = 'eb62f6b9306db575c2d596b1279627a4'  # Hash 结果

print(next(i for i in (''.join(i) for i in itertools.product(char, repeat=length)) if func(i.encode()).hexdigest() == result))
