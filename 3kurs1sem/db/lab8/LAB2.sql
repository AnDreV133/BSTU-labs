-- SCHEMA: main

-- DROP SCHEMA IF EXISTS main ;

-- CREATE SCHEMA IF NOT EXISTS main
--     AUTHORIZATION postgres;

DROP TABLE IF EXISTS purchase;
DROP TABLE IF EXISTS accounting;
DROP TABLE IF EXISTS check_list CASCADE;
DROP TABLE IF EXISTS store CASCADE;
DROP TABLE IF EXISTS product CASCADE;

-- таблица магазинов
CREATE TABLE IF NOT EXISTS store (
	id BIGSERIAL PRIMARY KEY, 
	address VARCHAR (100) 
);

-- таблица продукции, поставляемой в магазины 
CREATE TABLE IF NOT EXISTS product (
	article BIGSERIAL PRIMARY KEY, 
	name VARCHAR (100) NOT NULL,
	category VARCHAR (10), 
	quantity_to_assess VARCHAR (10) NOT NULL 
);

-- таблица чеков со связью с магазином
CREATE TABLE IF NOT EXISTS check_list (
	id BIGSERIAL NOT NULL PRIMARY KEY, 
	store_id BIGSERIAL REFERENCES store (id) 
		ON DELETE SET NULL, 	-- если удалить магазин, 
								-- то чек останется
	time TIMESTAMP NOT NULL
);

-- таблица учёта
CREATE TABLE IF NOT EXISTS accounting (
	id BIGSERIAL NOT NULL PRIMARY KEY, 
	store_id BIGSERIAL NOT NULL REFERENCES store (id) 
		ON DELETE CASCADE, -- нет магазина - нет учёта
	product_article BIGSERIAL NOT NULL REFERENCES product (article) 
		ON DELETE CASCADE, -- нет товара - нет строчки учёта
	cost NUMERIC(6, 2),
	amount INTEGER NOT NULL default 0
);

-- покупка является строкой в чеке
CREATE TABLE IF NOT EXISTS purchase (
	id BIGSERIAL NOT NULL PRIMARY KEY, 
	check_list_id BIGSERIAL NOT NULL REFERENCES check_list (id) 
		ON DELETE CASCADE, 	-- нет чека - нет покупок по нему
	product_article BIGSERIAL NOT NULL REFERENCES product (article) 
		ON DELETE CASCADE, 	-- нет товара, значит невозможно 
							-- рассчитать его цену
	amount NUMERIC(6, 2) NOT NULL 	-- тип данных для точного
									-- представления количества товара
);




