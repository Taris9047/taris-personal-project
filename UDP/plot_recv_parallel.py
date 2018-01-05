#!/usr/bin/env python

###################################################
#                                                 #
# data_process benchmark data parser.             #
#                                                 #
# Parses data for multi-process case obtained by  #
# "run_parallel_all.sh" script.                   #
#                                                 #
###################################################

import os
from os import listdir
from os.path import isfile, join

results_dir = "./data_process/results"
data_files = [ f for f in listdir(results_dir) if isfile(join(results_dir, f)) ]
outf_name = "./recv_benchmark.txt"

n_thread_list = [1, 2, 3, 4, 5, 6, 7, 8, 16, 24, 32, 48, 72, 96, 128, 192]
# tbl_col_ind_map = {'pgi': 2, 'gcc': 3, 'llvm': 4}

'''@Data file information container class

'''
class DataFile:
    prefix = ''
    data_file_name = ''

    compiler = ''
    process_id = 0

    def __init__(self, data_fn, df_prefix):
        self.data_file_name = data_fn
        self.prefix = df_prefix

        if (self.data_file_name == ''):
            raise ValueError("Data file name isn't given!!")

        self.compiler = self.data_file_name.split('_')[0]
        proc_txt = self.data_file_name.split('_')[2]
        self.process_id = int(proc_txt.split('proc')[-1])

    def GetCompiler(self):
        return self.compiler

    def GetProcessId(self):
        return self.process_id


'''@Data
    Class contains each section of data
'''
class Data:
    Address_list = []
    n_threads = 0
    run_date = ''
    process_id = 0
    bit_rate = 0

    def __init__(self, addrs, n_thr, run_date, proc_id, br):
        self.Address_list = addrs
        self.n_threads = int(n_thr)
        self.run_date = str(run_date)
        self.process_id = int(proc_id)
        self.bit_rate = int(br)


'''@DataFile parser class

'''
class DataFileParser:
    df_cont = None

    DataList = []
    Compiler = ''

    def __init__(self, df_container):
        self.df_cont = df_container
        if (self.df_cont == None):
            raise ValueError("Invalid DataFile class given!!")
        self.Compiler = self.df_cont.compiler
        self.Parse()

    def Parse(self):
        file_path = join(self.df_cont.prefix, self.df_cont.data_file_name)
        with open(file_path, 'r') as fp:
            l = fp.readline()
            while l:
                if "section start" in l.lower():
                    fp.readline()
                    addr_list = self.ParseAddr(fp.readline())
                    nthreads = fp.readline().split(': ')[-1]
                    run_date = fp.readline().split(': ')[-1]
                    pid = fp.readline().split(': ')[-1]
                    br = fp.readline().split(' ')[-2]
                    data = Data(addr_list, nthreads, run_date, pid, br)
                    self.DataList.append(data)
                l = fp.readline()

    def ParseAddr(self, addr_str):
        return addr_str.split(' ')

'''@Table
    a data table class

'''
class Table:
    rows = 0
    cols = 0
    tbl = {}

    def __init__(self):
        rows = cols = 0

    def Get(self, row, col):
        return self.__search(row, col)

    def Insert(self, row, col, element):
        self.tbl[(row, col)] = element
        if row > self.rows:
            self.rows = row
        if col > self.cols:
            self.cols = col

    def InsertAdd(self, row, col, element):
        e = self.__search(row, col)
        if e is None:
            self.Insert(row, col, element)
        else:
            self.Insert(row, col, element+e)

    def __search(self, row, col):
        coords = sorted(self.tbl.keys())
        if coords == []:
            return None

        for c in coords:
            if c[0] == row and c[1] == col:
                return self.tbl[c]
        return None

    def CSVstr(self, delimiter=','):
        csv_str = ''
        for i in range(1, self.rows+1):
            for j in range(1, self.cols+1):
                e = self.__search(i, j)
                if e == None:
                    csv_str += delimiter
                else:
                    csv_str += str(e) + delimiter

                if j == self.cols:
                    csv_str += '\n'

        return csv_str


'''@Benchmark data generation

                row_ind = n_thread_list.ind
    TODO: Finish this crap tomorrow!!
'''
class BenchData:
    DataTable = Table()
    outf_name = ''
    prefix = ''

    def __init__(self, df_list, prefix, outf):
        self.outf_name = outf
        self.prefix = prefix

        # Now, populate the table
        # Generating header
        self.DataTable.Insert(1,1, 'threads')
        self.DataTable.Insert(1,2, 'bps(PGI)')
        self.DataTable.Insert(1,3, 'bps(gcc)')
        self.DataTable.Insert(1,4, 'bps(llvm)')

        # Filling out contents.
        for df in df_list:
            DFP = DataFileParser(DataFile(df, prefix))

            if 'pgi' in DFP.Compiler.lower():
                col_ind = 2
            elif 'gcc' in DFP.Compiler.lower():
                col_ind = 3
            elif 'llvm' in DFP.Compiler.lower():
                col_ind = 4
            elif 'clang' in DFP.Compiler.lower():
                col_ind = 4

            for d in DFP.DataList:
                row_ind = n_thread_list.index(d.n_threads)+2
                self.DataTable.Insert(row_ind, 1, d.n_threads)
                self.DataTable.InsertAdd(row_ind, col_ind, d.bit_rate)

    def Write(self):
        if self.outf_name == '':
            raise ValueError("Output file name was not given!!")
        tbl_str = self.DataTable.CSVstr()

        with open(join('./', self.outf_name), 'w') as fp:
            fp.write(tbl_str)


'''@Main operation

'''
if __name__ == "__main__":
    bd = BenchData(data_files, results_dir, outf_name)
    bd.Write()
