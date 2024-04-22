1.查询每个制造商及其生产的最低价格的笔记本型号

```sql
SELECT   maker,model FROM  ( SELECT * FROM (SELECT maker,Laptop.model,price
FROM Product
JOIN Laptop ON Product.model = Laptop.model
WHERE Product.type = 'Laptop')  as a 
having 1 
order by price asc) as b
GROUP BY maker
```

2.查询生产的笔记本的硬盘容量不小于100GB的制造商

```sql
SELECT DISTINCT maker
FROM Product INNER JOIN Laptop ON Product.model = Laptop.model
WHERE hd >= 100
```



3.查询生产最快速度的计算机的制造商

```sql
select distinct maker
from product
where model in (
    select a.model model
    from(
        select model,speed
        from pc 
        union 
        select model,speed 
        from laptop
    ) a 
    where a.speed=(
        select max(b.speed)
        from(
            select model,speed
            from pc 
            union 
            select model,speed 
            from laptop
        )b
    )
)


```

4.将所有内存容量为1024的笔记本价格下调200元

```sql
UPDATE Laptop
SET price = price - 200
WHERE ram = 1024
```

5.用差集法查询选修了全部课程的学生姓名

```sql
SELECT Student.Sname
FROM Student
EXCEPT
SELECT Student.Sname
FROM Student
JOIN SC ON Student.Sno = SC.Sno
JOIN Course ON SC.Cno = Course.Cno
GROUP BY Student.Sname
HAVING COUNT(*) < (SELECT COUNT(*) FROM Course)

```

