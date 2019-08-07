// Seqpk.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../include/zlib.h"
#include "./kseq.h"
using namespace std;

KSEQ_INIT(gzFile, gzread)

inline bool endsWith(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int main(int argc,char *argv[] )
{
    //std::cout << "Hello World!\n";
	if (argc != 3) {
		std::cerr << "Usage: Seqpk.exe <fq file> <sampling size>" << endl;
		return 1;  // Invalid number of arguments.
	}
	else {

		gzFile fq;
		ofstream outfile;
		if (endsWith(argv[1], ".gz")) {
			fq = gzopen(argv[1], "r");
			string headname = argv[1];
			int samsize = atoi(argv[2]);
			
			string dire;
			const size_t last_slash_idx = headname.rfind('\\');
			if (std::string::npos != last_slash_idx)
			{
				dire = headname.substr(0, last_slash_idx);
			}

			string title;
			size_t tol = headname.length();
			title = headname.substr(last_slash_idx + 1, tol - 7 - last_slash_idx);

			outfile.open(dire + "\\" + "Subsampled_" + title + ".fq.gz");

			kseq_t *seq;
			seq = kseq_init(fq);

			int n = 0;
			while (kseq_read(seq) >= 0) {
				++n;
				if (n % samsize == 0) {
					//outfile << seq->name.s <<endl;
					//outfile << seq->comment.s << endl;
					//outfile << seq->seq.s << endl;
					//outfile << seq->qual.s << endl;
					outfile << '@';
					outfile << seq->name.s << '\n';
					outfile << seq->seq.s << '\n';
					outfile << "+" << '\n';
					outfile << seq->qual.s << '\n';

				}

			}
			//std::cout << "Total n. of sequences: " << n << '\n';
			kseq_destroy(seq);
			gzclose(fq);
			outfile.close();
		}
		else {
			fprintf(stderr, "Note: It is not fq.fz file .\n");
			return 1;

		}
	}
}




