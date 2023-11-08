#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct KeyMap {
	char og[52];
	char ng[52];
};

int check_key(string key);
struct KeyMap make_keymap(string user_key);
void sub(struct KeyMap map, string plaintext);

int main(int argc, string argv[]){

	// Check number of cmd arguments
	if (argc != 2){
		printf("Invalid number of arguments\n");
		return 1;
	}

	// check key has all letters and no extras/repeats
	int key_flag = check_key(argv[1]);
	if (key_flag != 0){
		printf("Invalid key\n");
		return 1;
	}
	struct KeyMap usermap = make_keymap(argv[1]);
	string text = get_string("Plain text:  \n");
	printf("ciphertext: ");

	sub(usermap, text);
	printf("\n");
	return 0;

}

int check_key(string key){

	int x = strlen(key);
	if (x != 26){
		return 1;
	}
	for (int i = 0; i < 26; i++){
		if (isalpha(key[i]) == 0){
			return 1;
		}
		char c = tolower(key[i]);
		for (int j = i + 1; j < 26; j++){
			char compare = tolower(key[j]);
			if (compare == c)
			{
				return 1;
			}
		}
	}

	return 0;
}

struct KeyMap make_keymap(string user_key){
	struct KeyMap user_map;
	for (int i = 0; i < 26; i++){
		user_map.og[i] = i + 65;
		user_map.og[i + 26] = i + 97;
		user_map.ng[i] = toupper(user_key[i]);
		user_map.ng[i + 26] = tolower(user_key[i]);
	}
	return user_map;
}

void sub(struct KeyMap map, string plaintext){
	for (int i = 0, n = strlen(plaintext); i < n; i++){
		char c = plaintext[i];
		if (c < 60 || c > 122 || (c > 90 && c < 97)){
			printf("%c", c);
		}
		else {
			for (int j = 0; j < 52; j++){
				if (c == map.og[j]){
					printf("%c", map.ng[j]);
				}
			}
		}
	}
}

}

