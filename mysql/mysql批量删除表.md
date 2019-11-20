mysql批量删除表

Select CONCAT( 'drop table ', table_name, ';' ) FROM information_schema.tables Where table_name LIKE 'hydra_%';

