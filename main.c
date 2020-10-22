#define _POSIX_C_SOURCE 200112L

#include <string.h>

#define SUCCESS 0
#define ERROR 1


int main(int argc, char const *argv[]){ 
	if (argc > 6 || argc < 2){   //verificar que estén bien estos números
		perror("Invalid command");
		return ERROR;
	}else{
		//bool error = false;  //ver si sacar después en caso de que no me sirva
		if (argc == 2){
			if (argv[1] == "-V"){  //dejo tambien que lo pongan en minúscula?
				printf("%s", "Versión 1.2"); //qué version habría que poner??
				return SUCCESS;
			}
			if (argv[1] == "-h"){
				printf("%s", "Usage:");   //Lo imprimo así o todo en una línea con barras n?
				printf("%s", "tp0 -h");
				printf("%s", "tp0 -V");
				printf("%s", "tp0 [options]");
				printf("%s", "-V, --version Print version and quit.");
				printf("%s", "-h, --help Print this information.");
				printf("%s", "-o, --output Path to output file.");
				printf("%s", "-i, --input Path to input file.");
				printf("%s", "-d, --decode Decode a base64-encoded file.");
				return SUCCESS;
			}
			perror("Invalid command");//HAGO UN STDERR??? asi ta bien??
			return ERROR;
		}
		char* input_file;
		char* output_file;
		bool decode = false;
		bool error = false;
		if (argv[1] == "-d"){
			decode = true;
			if (argc == 4){
				input_file = NULL;
				output_file = argv[3]; 
			}
			else{
				input_file = argv[3];
				output_file = argv[5];
			} 
		}
		else if (argv[1] == "-i"){
			input_file = argv[2];
			output_file = argv[4];
		}
		else if (argv[1] == "-o"){
			input_file = NULL;
			output_file = argv[2];
		}else{
			perror("Invalid command");//HAGO UN STDERR??? asi ta bien??
			return ERROR;  // o puede haber otra alternativa??
		}
		if (decode){
			base_64_decoder_t decoder;
			if (base_64_decoder_init(&decoder, input_file)==ERROR){
				perror("Error initiating decoder");//HAGO UN STDERR??? asi ta bien??
				return ERROR;
			}
			if (base_64_decoder_decode(&decoder) == ERROR)
				bool error = true;
			base_64_decoder_destroy(&decoder);
			if (error){
				perror("Error decoding");//HAGO UN STDERR??? asi ta bien??
				return ERROR;
			}
			return SUCCESS;
		}  
		else{

		}
		//return SUCCESS;
	}



// OTRA OPCION CON UN POQUITO MENOS DE REPETICIÓN DE CÓDIGO
		// char* input_file;
		// char* output_file;
		// bool decode = false;
		// inf offset = 0;
		// if (argv[1] == "-d"){
		// 	decode = true;
		// 	offset = 1;
		// }
		// else if (argv[1 + offset] == "-i"){
		// 	input_file = argv[2 + offset];
		// 	output_file = argv[4 + offset];
		// }
		// else if (argv[1 + offset] == "-o"){
		// 	input_file = NULL;
		// 	output_file = argv[2 + offset];
		// }else{
		// 	perror("Invalid command");//HAGO UN STDERR??? asi ta bien??
		// 	return ERROR;  // o puede haber otra alternativa??
		// }
		// if (decode) decode(input_file, output_file); 
		// else encode(input_file, output_file);
		// return SUCCESS;
}
