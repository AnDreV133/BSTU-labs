

select 	t2.category, 
		t2.name, 
		sum(t1.amount) as amount 
	from check_list as t0 
	inner join purchase as t1
	on t0.id=t1.check_list_id
	inner join product as t2
	on t1.product_article=t2.article
	group by t2.category, t2.name
	order by t2.category desc, amount desc;


select  t2.name, 
		round(coalesce(sum(t1.amount)*100/(select sum(amount) from purchase),0),2) as amount_in_percent 
	from check_list as t0 
	inner join purchase as t1
	on t0.id=t1.check_list_id
	right join product as t2
	on t1.product_article=t2.article
	group by t2.name
	order by amount_in_percent desc;


select 	t0.store_id, 
		round(avg(t1.amount*t2.cost), 2) as avg_sum_of_checks
	from check_list as t0 
	inner join purchase as t1
	on t0.id=t1.check_list_id 
	inner join accounting as t2
	on t1.product_article=t2.product_article
	where t0.time<now()
	group by t0.store_id
	having t0.store_id=1;


