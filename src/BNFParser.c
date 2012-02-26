#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include"BNFParser.h"


BNFParser* BNFParser__new(CGArray(BNFToken)* tokens, BNFSentence* startSentence);
void BNFParser_delete(Parser* parser);
