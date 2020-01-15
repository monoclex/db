# db - Unix Philosophy Based Database

`db` is a simple tool, written in C, for purely hobby purposes. The primary goal is to implement databases in a stupidly easy way.

DB Structure:
```
/db_name (the name of the database)
|
>- /tags (contains every tag as a folder)
|  |
|  >- /tag_name1 (contains a symlink with each file being the name of the item in `/db_name/data` that is being linked to)
|  |  |
|  |  >- GUID_GOES_HERE (a symlink to `/db_name/data/GUID_GOES_HERE`)
|  |
|  >- /another_tag_name
|     |
|     >- MORE_GUIDS_HERE
|
>- /data (contains every entry in the database)
   |
   >- GUID_GOES_HERE
   |
   >- MORE_GUIDS_HERE
```

Commands:
## db new <db_name>

Creates the following folder structure
```
/db_name
|
>- /tags
|
>- /data
```

## db <db_name> new-tag <tag_name>

Creates the following folder
```
/db_name
|
>- /tags
|  |               <-- NEW
|  >- /<tag_name>  <-- NEW
|
>- /data
```

## db <db_name> insert <file>

Moves `file` into `/db_name/data` with a GUID ID. The GUID ID chosen for the file can be obtained by observing the output of STDOUT.

tl;dr
```
root@linux:~$ db test_db insert my_file
FB0542D9-9D69-A313-CA55-85195A974D59
```

```
/db_name
|
>- /tags
|  |
|  >- /some_tag
|
>- /data
   |                                         <-- NEW
   >- /FB0542D9-9D69-A313-CA55-85195A974D59  <-- NEW
```

## db <db_name> tag <tag_name> <guid>

Creates a symlink from the file in `/db_name/data/<guid>` to `/db/tags/<tag_name>/<guid>`.

```
/db_name
|
>- /tags
|  |
|  >- /some_tag
|     |  (links to /db_name/data/FB0542D9-...)  <-- NEW
|     >- /FB0542D9-9D69-A313-CA55-85195A974D59  <-- NEW
|
>- /data
   |
   >- /FB0542D9-9D69-A313-CA55-85195A974D59
```