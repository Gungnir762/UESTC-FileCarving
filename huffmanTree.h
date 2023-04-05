#pragma once
#include <cstdint>
#include<set>
#include"FindJPG.h"
#include"BasicFunction.h"
#define USE_HUFFMAN_CHECK 0

typedef struct tree {
	int ch[2][500];
	int cnt;//[1,cnt]代表各节点编号
	int root;//根节点编号，初始化时为0，正常情况下为1

	tree() {
		cnt = 0;
		root = 0;
		memset(ch, 0, sizeof(ch));
	}

	void clear() {
		cnt = 0;
		root = 0;
		memset(ch, 0, sizeof(ch));
	}

	void add_chain(string str) {
		if (!root) {
			cnt++;
			root = cnt;
		}
		int cur = root;
		for (int i = 0; i < str.length(); i++) {
			int nxt = str[i] - '0';
			if (i == str.length() - 1) {
				if (ch[nxt][cur]) {
					printf_s("哈弗曼编码有误，存在前缀码\n");
				}
				ch[nxt][cur] = root;
			}
			else {
				if (ch[nxt][cur]) {
					cur = ch[nxt][cur];
				}
				else {
					cnt++;
					ch[nxt][cur] = cnt;
					cur = ch[nxt][cur];
				}
			}
		}
	}

	void load_chains(set<string> s) {
		for (auto str : s) {
			add_chain(str);
		}
	}

	int check(int start, string str) {
		int cur = start;
		for (int i = 0; i < str.length(); i++) {
			int nxt = str[i] - '0';
			if (ch[nxt][cur] == 0) return 0;
			cur = ch[nxt][cur];
		}
		return 1;
	}

}tree;

void load_huffman_code_set(tree& T, set<string>huffman_code_set) {
	T.clear();
	T.load_chains(huffman_code_set);
}

void append_byte(string& str, unsigned char c) {
	int tmp = c;
	for (int i = 0; i < 8; i++) {
		if (tmp & 1) str.push_back('1');
		else str.push_back('0');
		tmp >>= 1;
	}
}

tree HuffmanTree;//重新申请空间效率太低，直接申明为全局变量
//判断这个扇区是不是jpg的一部分
//buffer：一个扇区的起始
//返回值：0：是jpg的一部分，且未结束；
//        1：是jpg的一部分，且在第一个ffd9处结束；
//        -1：不是jpg的一部分。
//注：不能判断FFD9正好被扇区分割的情况
int is_jpg_sector(unsigned char* buffer, set<string>huffman_code_set) {
	int f = 0;
	int warning = 0;
	string str;
	for (int i = 0; i < SECTOR_SIZE; i++) {
		if (warning) {
			if (buffer[i] == '\x00') {//读到FF后读到00，则丢弃00
				warning = 0;
				append_byte(str, (unsigned char)'\xFF');
			}
			else if (buffer[i] == '\xD9') {//读到FF后读到D9，则认为jpg结束
				f = 1;
				for (int j = 0; j < 7; j++) {
					if (!str.empty()) str.pop_back();//FFD9要与字节对齐，则前面至多有7位不在huffman序列内
				}
				break;
			}
			else {//读到FF后读到其它字节，则认为不合法
				return -1;
			}
		}
		else {
			if (buffer[i] == '\xFF') {
				warning = 1;
				continue;
			}
			append_byte(str, buffer[i]);
		}
	}

	//由于data段内数据的组织方式还有待考证，这部分应慎用
	if (USE_HUFFMAN_CHECK) {
		load_huffman_code_set(HuffmanTree, huffman_code_set);
		for (int i = 1; i <= HuffmanTree.cnt; i++) {
			if (HuffmanTree.check(i, str)) return f;
		}
		return -1;
	}
	return f;
}
