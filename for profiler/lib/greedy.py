import plotly.graph_objects as go

class GreedySpline:
    def __init__(self, error=10):
        self.error=error
        self.greedyspline=[]

    def start(self,new_point):
        self.tg_err_top=float("inf")
        self.tg_err_bot=float("-inf")
        self.greedyspline.append(new_point)
        self.greedyspline.append(new_point)
        self.last_point=new_point
    
    def tg_spline(self, a,b,err=0):
        return (a[1]+self.error*err-b[1])/(a[0]-b[0])
        
    def new_coridor (self,a,b):
        self.tg_err_top=min(self.tg_err_top, self.tg_spline(a,b,1))
        self.tg_err_bot=max(self.tg_err_bot, self.tg_spline(a,b,-1))
        
    def new_spline(self, new_point): 
        if not self.greedyspline:
            self.start(new_point)
            return
        tg_new_point=self.tg_spline(new_point,self.greedyspline[-2])
        if tg_new_point<self.tg_err_top and tg_new_point>self.tg_err_bot:
            self.new_coridor(new_point, self.greedyspline[-2])
            self.greedyspline[-1]=new_point
        else:
            self.greedyspline.append(self.last_point)
            self.tg_err_top=self.tg_spline(new_point, self.greedyspline[-1],1)
            self.tg_err_bot=self.tg_spline(new_point, self.greedyspline[-1],-1)
        self.last_point=new_point 
        
        
        
    def load_data(self,data):
        for i in data:
            self.new_spline(i)
    
    def plot(self,fig):
        dataX=[]
        dataY=[]
        for i,j in  self.greedyspline:
            dataX.append(i)
            dataY.append(j)
        fig.add_trace(go.Scatter(x=dataX,y=dataY, name="G",  mode='lines+markers',
                marker=dict(
                    color="green",
                    size=10,
                ),))
        return fig
    
        
        
    

