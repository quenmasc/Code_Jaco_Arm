import os ,time

fifo_name= 'fifo'

def main():
 
    try :
        os.mkfifo("../cpp/%s"%fifo_name)
    except FileExistsError :
        pass
    while True:
        data=b'1'
        with open(fifo_name,'wb') as f:
                f.write('{}\n'.format(len(data)).encode())
                f.write(data)
        f.flush
        time.sleep(0.2)

if __name__ == '__main__' :
    main()
