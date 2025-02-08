import plotly.graph_objects as go

def tg(left_point, right_point):
    if left_point[0]==right_point[0]:
        tg=float("inf")*(-left_point[1]+right_point[1])
    else:
        tg=(left_point[1]-right_point[1])/(left_point[0]-right_point[0])
    return tg



class One_Line:
    def __init__ (self,start_point,err):
        self.start_point=start_point
        self.finish_point=start_point
        # self.finish_point=[start_point[0]+1,start_point[1]+1]
        self.err=err
        self.points=[self.start_point,self.finish_point]
        s_a,s_b=self.err_diapason(start_point)
        s_d,s_c=self.err_diapason(self.finish_point)
        self.pmin=tg(s_a,s_c)
        self.pmax=tg(s_b,s_d)
        self.cvx_top=[s_a]
        self.cvx_bot=[s_b]
        
        # print(self.pmin, self.pmax)
        
    def err_diapason(self, point):
        plus_err=[point[0],point[1]+self.err]
        minus_err=[point[0],point[1]-self.err]
        return plus_err,minus_err
    
    def check_not_outside(self, plus_err,minus_err):
        return tg(self.cvx_top[0],minus_err)<=self.pmax and tg(self.cvx_bot[0],plus_err)>=self.pmin
    
    def show_spline(self, fig):
        # step_right=1
        # fig=go.Figure()
        for i in self.points:
            fig=self.plot_point(fig,i)
        # fig=self.plot_line(fig,self.points, "points")
        # for i in self.err_array(self.points):
        #     fig=self.plot_line(fig,i, "err")
        fig=self.plot_line(fig,[self.cvx_bot[0],self.cvx_top[-1]], "pmax")
        fig=self.plot_line(fig,[self.cvx_top[0],self.cvx_bot[-1]], "pmin")
        fig=self.plot_line(fig,[self.startLine,self.endLine], "spline")
        # fig=self.plot_line(fig,[self.cvx_top[0],self.cvx_bot[-1]], "p")
        # fig.show()
        return fig
        
    def plot_point(self, fig, point):
        plus_err,minus_err=self.err_diapason(point)
        
        # fig=px.scatter(self.points)
        fig=self.plot_line(fig,[plus_err,point,minus_err],"",showlegend=False )
        return fig
        
    
    def     plot_line(self, fig, line, name, showlegend=False,  width=2):
        x_=[]
        y_=[]
        for i in line:
            x_.append(i[0])
            y_.append(i[1])
            
        
        # fig=px.scatter(self.points)
        fig.add_trace(go.Scatter(x=x_,y=y_, name=name,showlegend=showlegend, fillcolor="RED"))
        return fig
        
    def add_point(self,point):
        plus_err,minus_err=self.err_diapason(point)
        result=self.check_not_outside(plus_err,minus_err)
        if result:
            self.points.append(point)
            self.add_top_err_point(plus_err)
            self.add_bot_err_point(minus_err)
            self.finish_point=point
        else:
            self.end_line()
        return result
    
    
    def end_line(self):
        intersection_point=self.intersection_point()
        if intersection_point!=None:
                intersection_point=self.intersection_point()
                p=(self.pmin+self.pmax)/2
                self.startLine=self.point_on_line(intersection_point, p, self.start_point[0])
                self.endLine=self.point_on_line(intersection_point, p, self.finish_point[0])
        else:
            self.startLine=self.endLine=self.start_point
            
    def point_on_line(self, intersection_point, p, target_x):
        x0,y0=intersection_point
        b = y0 - p * x0
        y = p * target_x + b
        return (target_x, y)
            
    def intersection_point(self):
        x1, y1 = self.cvx_top[0]
        x2, y2 = self.cvx_bot[-1]
        x3, y3 = self.cvx_bot[0]
        x4, y4 = self.cvx_top[-1]
        denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
        if denominator == 0:
            return None
        x_numerator = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)
        y_numerator = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
        x = x_numerator / denominator
        y = y_numerator / denominator

        return (x, y)
   
    def add_bot_err_point(self, new_point_bot_err):
        
        if tg(self.cvx_top[0], new_point_bot_err)> self.pmin:
            
            temp_pmin=tg(self.cvx_top[0], new_point_bot_err)
            ln=len(self.cvx_top)
            while ln>1 and tg(self.cvx_top[1], new_point_bot_err)> temp_pmin:
                temp_pmin=tg(self.cvx_top[1], new_point_bot_err)
                self.cvx_top.pop(0)
                ln-=1
            self.pmin=temp_pmin
            
        ln=len(self.cvx_bot)
        last_p=tg(self.cvx_bot[-1], new_point_bot_err)
        while ln>1 and tg(self.cvx_bot[-2], new_point_bot_err)< last_p:
            last_p=tg(self.cvx_bot[-2], new_point_bot_err)
            self.cvx_bot.pop()
            ln-=1
        self.cvx_bot.append(new_point_bot_err)
            
    def add_top_err_point(self, new_point_top_err):
        
        if tg(self.cvx_bot[0], new_point_top_err)< self.pmax:
            temp_pmax=tg(self.cvx_bot[0], new_point_top_err)
            ln=len(self.cvx_bot)
            # print(self.cvx_bot)
            while  ln>1 and tg(self.cvx_bot[1], new_point_top_err)< temp_pmax:
                temp_pmax=tg(self.cvx_bot[1], new_point_top_err)
                self.cvx_bot.pop(0)
                ln-=1
            self.pmax=temp_pmax
        ln=len(self.cvx_top)    
        last_p=tg(self.cvx_top[-1], new_point_top_err)
        while  ln>1 and tg(self.cvx_top[-2], new_point_top_err)> last_p:
            last_p=tg(self.cvx_top[-2], new_point_top_err)
            self.cvx_top.pop()
            ln-=1
        self.cvx_top.append(new_point_top_err)
            # print(self.cvx_bot)
            # print(self.cvx_top)
        
        
def PLR(data, err, show=False):
    line=One_Line(data[0],err=err)
    PLRSpline=[]
    fig=go.Figure()
    for i in data[1:]:
        if not line.add_point(i):
            # print("New_line")
            if show: 
                fig=line.show_spline(fig)
            PLRSpline.append([line.startLine, line.endLine])
            line=One_Line(i,err=err)
    line.end_line()
    PLRSpline.append([line.startLine, line.endLine])
    if show: 
        fig=line.show_spline(fig)
        fig.show()
    return PLRSpline