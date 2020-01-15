// #define DEBUG

#ifdef DEBUG
#define FUNCTION_IN(function_name) printf("in  %s\n", (function_name));
#define FUNCTION_OUT(function_name) printf("out %s\n", (function_name));
#define NULL_CHECK(item, name) if ((item) == NULL) { printf("NULL -> %s", (name)); }
#else
#define FUNCTION_IN(function_name) /**/
#define FUNCTION_OUT(function_name) /**/
#define NULL_CHECK(item, name) /**/
#endif

#include "random.c"
#include "guid.c"
#include "errors.c"
#include "strconcat.c"
#include "filesystem.c"

#include <stdint.h> // int32_t and friends
#include <stdio.h> // printf

static int32_t db_new(const char* const db_name) {
	FUNCTION_IN("db_new");
	int32_t error = 0;

	char* path = path_full(db_name);
	error += make_dir(path);

	char* dir_tags = CONCAT2(path, "/tags");
	char* path_tags = path_full(dir_tags);
	error += (make_dir(path_tags) << 1);
	free(dir_tags);
	free(path_tags);

	char* dir_data = CONCAT2(path, "/data");
	char* path_data = path_full(dir_data);
	error += (make_dir(path_data) << 2);
	free(dir_data);
	free(path_data);

	free(path);

	FUNCTION_OUT("db_new");
	return error;
}

static int32_t db_new_tag(const char* const db_name, const char* const tag_name) {
	int32_t error = 0;

	char* path = path_full(db_name);
	char* tag_path = CONCAT3(path, "/tags/", tag_name);

	error = make_dir(tag_path);

	free(tag_path);
	free(path);

	return error;
}

static int32_t db_tag(const char* const db_name, const char* const tag, const char* const item_id) {
	int32_t error = 0;

	char* dir_tag_link = CONCAT5(db_name, "/tags/", tag, "/", item_id);
	char* path_tag = path_full(dir_tag_link);
	free(dir_tag_link);

	char* dir_data_link = CONCAT3(db_name, "/data/", item_id);
	char* path_data = path_full(dir_data_link);
	free(dir_data_link);

	error = make_link(path_data, path_tag);
	
	free(path_data);
	free(path_tag);

	return error;
}

static int32_t db_insert(const char* const db_name, const char* const file_name) {
	int32_t error = 0;

	guid_t guid;
	random_fill(&guid, sizeof(guid_t));
	char* guid_string = alloc_guid_to_string(guid);

	printf("%s\n", guid_string);

	char* data_path = CONCAT3(db_name, "/data/", guid_string);
	error = move(file_name, data_path);

	free(data_path);
	free(guid_string);

	return error;
}

// used within `main`
#define ARGIS(index, checking) (0 == strcmp(ARGAT((index)), (checking)))
#define ARGAT(index) (argc > index ? argv[index] : arg_err(argc, argv, (index)))
static char* arg_err(int argc, char** argv, int32_t index) {
	printf("can't get arg at '%i'\n", index);
	exit(NotEnoughArguments);
	return NULL;
}

int main(int argc, char** argv) {
	FUNCTION_IN("main");
	if (argc <= 1) {
		// TODO: figure out default thing for `db`
		// printf("too few args\n");
		guid_t guid;
		random_fill(&guid, sizeof(guid_t));
		printf("%s\n", alloc_guid_to_string(guid));
		return 0;
	}

	if (ARGIS(1, "new")) {
		return db_new(ARGAT(2));
	}

	char* db_name = ARGAT(1);

	if (ARGIS(2, "new-tag")
		|| ARGIS(2, "tag-new")) {
		return db_new_tag(db_name, ARGAT(3));
	}

	if (ARGIS(2, "tag")) {
		return db_tag(db_name, ARGAT(3), ARGAT(4));
	}

	if (ARGIS(2, "insert")
		|| ARGIS(2, "add")) {
		return db_insert(db_name, ARGAT(3));
	}
}
