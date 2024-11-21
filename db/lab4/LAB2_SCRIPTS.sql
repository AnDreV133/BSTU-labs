-- SCHEMA: main

-- DROP SCHEMA IF EXISTS main ;

-- CREATE SCHEMA IF NOT EXISTS main
--     AUTHORIZATION postgres;

-- Инициализация 

DELETE FROM accounting;
DELETE FROM check_list;
DELETE FROM purchase;
DELETE FROM store;
DELETE FROM product;

INSERT INTO store 
		(id, address) 
	VALUES 
		(1, 'LA, 5 Avenue'), 
		(2, 'LA, 11 Avenue'),
		(3, 'LA, 12 Avenue');

INSERT INTO product 
		(article, name, category, quantity_to_assess)
	VALUES 
		(1, 'Колбаса докторская', 'meat', '100g'),
		(2, 'Сырок плавленный', 'milk', 'p'), 
		(3, 'Молоко, бутылка 1л', 'milk', 'p'),
		(4, 'Булочка "Лакомка"', 'bake', 'p'),
		(5, 'Стейк, говяжий', 'meat', 'p');

INSERT INTO accounting 
		(store_id, product_article, cost, amount)
	VALUES 
		(1, 1, 100, 700),
		(1, 2, 30, 300),
		(1, 3, 70, 300),
		(1, 4, 35, 200),
		(1, 5, 200, 100),
		(2, 1, 190, 700),
		(2, 2, 39, 300),
		(2, 3, 79, 300),
		(2, 4, 39, 200),
		(2, 5, 290, 100);


-- покупка

INSERT INTO check_list 
		(id, store_id, time)
	VALUES 
		(1, 1, now()-interval '8 hour'),
		(2, 1, now()-interval '6 hour'),
		(3, 1, now()),
		(4, 1, now()+interval '1 day'),
		(5, 2, now()),
		(6, 2, now())
	;

INSERT INTO purchase 
		(check_list_id, product_article, amount)
	VALUES 
		(1, 1, 4.5),
		(1, 2, 10);

INSERT INTO purchase 
		(check_list_id, product_article, amount)
	VALUES 
		(2, 4, 2),
		(2, 2, 3),
		(2, 3, 2);

INSERT INTO purchase 
		(check_list_id, product_article, amount)
	VALUES 
		(3, 4, 2),
		(3, 2, 30),
		(3, 3, 20),
		(3, 1, 4.5)
	;

INSERT INTO purchase 
		(check_list_id, product_article, amount)
	VALUES 
		(4, 2, 3),
		(4, 4, 8)
	;

INSERT INTO purchase 
		(check_list_id, product_article, amount)
	VALUES 
		(5, 3, 2);


INSERT INTO purchase 
		(check_list_id, product_article, amount)
	VALUES 
		(6, 4, 3);

	
-- UPDATE accounting
-- 	SET amount=amount-4.5
-- 	WHERE store_id=1 AND product_article=1;

-- UPDATE accounting
-- 	SET amount=amount-10
-- 	WHERE store_id=1 AND product_article=2;

select 
	accounting.store_id,
	accounting.product_article,
	round(purchase.amount*cost,2) as total_cost,
	time
	from check_list 
	inner join purchase 
	on check_list.id=purchase.check_list_id
	inner join accounting
	on purchase.product_article=accounting.product_article
	order by store_id asc, time desc
	;

-- select * from product;

-- select * from accounting;

-- select * from store;
