def main():
    hadoop_name = input('archivo de hadoop: ') or 'out.txt'
    res_name = input('archivo de resultado: ') or 'res.txt'
    msct = input('minimum support count: ') or '5000'
    msc = int(msct)

    result = open(res_name, 'w')
    for line in open(hadoop_name, 'r'):
        if int(line.split('\t')[1]) < msc:
            break
        result.write(line)

if __name__ == '__main__':
    main()
