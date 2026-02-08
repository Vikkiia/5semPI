  //10
  
  drop table BVV_T1
  
create table anyTable (
    id number
);
    

create view anyView as select * from anyTable;

commit;

drop view anyView;
drop table anyTable;


