from lib.greedy import *
import math



class RadixSpline:
    def __init__(self,GreedySpline,bitcount):
        self.bitcount=bitcount
        self.greedyspline=GreedySpline.greedyspline
        self.error=GreedySpline.error
        self.radixspline=[None]*2**self.bitcount
        self.bit_len=self.bit_lenght(self.greedyspline[-1][0])
        # print("bit_len",self.bit_len)
        self.len_greedy=len(self.greedyspline)
        self.not_prefics,self.mod=self.not_prefics_count()
        self.move_right=self.bit_len-self.bitcount-self.not_prefics
        # print("not_prefics",self.not_prefics)
        # print("mod", self.mod)
        # print("move_right",self.move_right)
        self.get_RadixSpline()
        
    def bit_lenght(self,n):
        return(len(bin(n).lstrip('-0b')))
    def not_prefics_count(self):
        n=self.greedyspline[0][0]
        count,pref=0,0
        
        if self.bit_lenght(n)==self.bit_len:
            count=self.bit_len-self.bit_lenght(((1<<self.bit_len)-1)^n)
        for i in range(count):
            pref=(pref<<1)+1
        pref<<=(self.bit_len-count)
        return count,pref
        
        
    
    ##Способ построение радикс сплайна, нужно заменить
    def first_bits(self,n):
        if self.not_prefics:
           n%=self.mod
        #    print(n)
        return n>>self.move_right
    
    def get_RadixSpline(self):
        
        for ind in range(len(self.greedyspline)):
            temp=self.first_bits(self.greedyspline[ind][0])
            # print(temp)
            if not self.radixspline[temp]:
                self.radixspline[temp]=ind
        if  self.radixspline[2**self.bitcount-1]==None:
             self.radixspline[2**self.bitcount-1]=len(self.greedyspline)-1
        for i in range (2**self.bitcount-1,-1,-1):
            if self.radixspline[i]==None:                
                self.radixspline[i]=self.radixspline[i+1]
        return self.radixspline
    
    def bin_search_spline(self,left,right,key):
        while left<=right:
            mid=(left+right)//2
            if key>= self.greedyspline[mid][0] :
                if key<=self.greedyspline[mid+1][0] :
                   return [mid,mid+1]
                else: 
                   left=mid+1
            else:
                right=mid-1


    
        
    def give_answer(self,key):
        first_bit=self.first_bits(key)
        left=self.radixspline[first_bit]-1
        if first_bit==len(self.radixspline)-1:
            right=len(self.greedyspline)-1
        else:
            right=self.radixspline[first_bit+1]
        # left=0
        # right=len(self.greedyspline)-1
        bin_ans=self.bin_search_spline(left,right,key)
        kl,pl=self.greedyspline[bin_ans[0]]
        kr,pr=self.greedyspline[bin_ans[1]]

        return pl+(key-kl)*(pr-pl)//max(1,(kr-kl))
    
    def plot(self, fig):
        
        dataX=[]
        dataY=[]
        for i in  self.radixspline:
            dataX.append(self.greedyspline[i][0])
            dataY.append(self.greedyspline[i][1])
        # plt.plot(dataX,dataY, marker='o', markersize=15)
        fig.add_trace(go.Scatter(x=dataX,y=dataY, name="R",
                                  mode='lines+markers',
                marker=dict(
                    color="red",
                    size=20,
                ),))
        return fig
    

def plot_data(data,fig,error=0 ):
    

    dataX=[]
    errT=[]
    errB=[]
    dataY=[]
    for i,j in  data:
        dataX.append(i)
        dataY.append(j)
        if error:
            errB.append(j-error)
            errT.append(j+error)
    fig.add_trace(go.Scatter(x=dataX,y=dataY, name="data",
                             mode="markers",
                marker=dict(
                    color="blue",
                    size=5,
                ),))
    
    # print(data.__sizeof__())
    if error:
        fig.add_trace(go.Scatter(x=dataX,y=errB, name="-err"))
        fig.add_trace(go.Scatter(x=dataX,y=errT, name="+err"))     
    return fig   

def get_spline(data, ERR,bitcount ,plot=[False,False]): 
    G=GreedySpline(error=ERR)
    G.load_data(data)
    
    # print(len(G.greedyspline)/len(data)) 
    R=RadixSpline(G, bitcount)
    if plot[0]:
        fig=go.Figure()
        
        fig=G.plot(fig)
        fig=R.plot(fig)
        fig=plot_data(data,fig,plot[1]*ERR )
        fig.show()
    return (G,R)

def bin_search(data, key, left, right):
    while left <= right:
        mid = (left + right) // 2  # Находим середину массив
        if data[mid][0] == key:  # Если целевое значение найдено
            return mid
        elif data[mid][0] < key:  # Если целевое значение справа
            left = mid + 1
        else:  # Если целевое значение слева
            right = mid - 1
    return -1  


def get_key(data, spline, key ):
    radix_answer=spline[1].give_answer(key[0])
    left=radix_answer-spline[0].error
    right=radix_answer+spline[0].error
    res=bin_search(data, key[0], max(left,data[0][1]),min(right, data[-1][1]))
    # print(res)
    if res!=key[1]: raise Exception("FAIL", res, key, math.floor(left),math.ceil(right))
    return res