import hashlib
import itertools
from string import digits, ascii_letters, punctuation
alpha_bet=digits+ascii_letters+punctuation
strlist = itertools.product(alpha_bet, repeat=4)

sha256="a645e3deef85766e43c8a1aa63d1f69eed55e7cb94f10973bd76a9ace57c7311"
tail="amLSvne0g1ypVG5J"

xxxx=''

for i in strlist:
    data=i[0]+i[1]+i[2]+i[3]
    data_sha=hashlib.sha256((data+str(tail)).encode('utf-8')).hexdigest()
    if(data_sha==str(sha256)):
        xxxx=data
        break

print(xxxx)