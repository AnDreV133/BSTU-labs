-- SCHEMA: main

-- DROP SCHEMA IF EXISTS main ;

CREATE SCHEMA IF NOT EXISTS main
    AUTHORIZATION postgres;

-- Инициализация 

INSERT INTO store 
		(id, address) 
	VALUES 
		(1, 'LA, 5 Avenue'), 
		(2, 'LA, 11 Avenue'),
		(3, 'LA, 12 Avenue');

INSERT INTO product 
		(article, name,  quantity_to_assess)
	VALUES 
		(1, 'Колбаса докторская', '100g'),
		(2, 'Сырок плавленный', 'p'),
		(3, 'Молоко бутылка, 1л', 'p');

INSERT INTO accounting 
		(id, store_id, product_article, cost, amount)
	VALUES 
		(1, 1, 1, 100, 700),
		(2, 1, 2, 30, 300),
		(3, 2, 2, 40, 350);


-- покупка

INSERT INTO check_list 
		(id, store_id, time)
	VALUES 
		(1, 1, NOW());

INSERT INTO purchase 
		(id, check_list_id, product_article, amount)
	VALUES 
		(1, 1, 1, 4.5),
		(2, 1, 2, 10);

UPDATE accounting
	SET amount=amount-4.5
	WHERE store_id=1 AND product_article=1;

UPDATE accounting
	SET amount=amount-10
	WHERE store_id=1 AND product_article=2;

DELETE FROM check_list WHERE id=1;

DELETE FROM product WHERE article=1;

DELETE FROM store WHERE id=1;

-- select * from accouting;

select * from check_list inner join purchase on check_list.id=purchase.check_list_id;

select * from product;

select * from accounting;

select * from store;
