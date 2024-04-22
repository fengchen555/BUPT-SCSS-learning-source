

1.找出在银行中同时有存款和贷款账户的客户（分别用带IN和EXIST谓词的嵌套查询完成

```sql
(In)
select customer-name
from depositor d
where d.account-number in (select account-number from account ) and d.customer-name in (select customer-name from borrower)
(EXIST)
select customer-name
from depositor d
where exist (select * from account a where d.account-number = a.account-name) 
and exist (select *from borrower b where d.cusotmer-name = b.customer-name)

```

2.使用ANY谓词找出那些总资产(assets)至少比位于Brooklyn的某一家分行要多的支行名字：

```sql
select branch-name 
from branch
where assets > ANY( select assets from branch where branch-city = 'Brooklyn')
```

3.找出平均存款余额(balance)最高的分行名字

```sql
select branch-name 
from account 
HAVING AVG(balance) >=  ALL(SELECT AVG(balance) FROM account GROUP BY branch-name)
```

