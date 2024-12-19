select * from purchase where check_list_id=4 order by product_article;

-- alter table check_list add index my_index (store_id)

create index my_index_1 on accounting (store_id);
create index my_index_2 on accounting (product_article);


-- чек
explain
select  t2.name as prod_name, 
        t1.amount,
        t2.quantity_to_assess,
        t1.amount*t3.cost as total_cost,
		(select time from check_list where id=4 limit 1) as time 
	from purchase as t1 
	inner join product as t2
		on t1.product_article=t2.article
	inner join accounting as t3
		on t1.product_article=t3.product_article
	where t1.check_list_id=4
	order by prod_name asc;

-- рейтинга товаров в своей категории
explain
select  t2.category, 
        t2.name, 
        sum(t1.amount) as amount 
    from check_list as t0 
    inner join purchase as t1
    on t0.id=t1.check_list_id
    inner join product as t2
    on t1.product_article=t2.article
    group by t2.category, t2.name
    order by t2.category desc, amount desc;


-- рейтинга всех товаров, с приведённым отношением количества определённой единицы на все товары
explain
select  t2.name, 
        round(coalesce(sum(t1.amount)*100/(select sum(amount) from purchase),0),2) 
            as amount_in_percent 
    from check_list as t0 
    inner join purchase as t1
    on t0.id=t1.check_list_id 
        and t0.store_id=1
    right join product as t2
    on t1.product_article=t2.article
    group by t2.name
    order by amount_in_percent desc;



-- Сумма среднего чека по определённому магазину за установленное время
explain
select  t0.store_id, 
        round(avg(t1.amount*t2.cost), 2) as avg_sum_of_checks
    from check_list as t0 
    inner join purchase as t1
    on t0.id=t1.check_list_id 
    inner join accounting as t2
    on t1.product_article=t2.product_article
    where t0.time<now()
    group by t0.store_id
    having t0.store_id=1;

