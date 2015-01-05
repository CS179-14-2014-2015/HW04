import java.awt.event.*;
import java.awt.*;
/**
 * Write a description of class Bullet here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Bullet 
{
    int x, y, radius, iteration, count, speed;
    double angle;
    Color c1;
    boolean circular, direction, stopChange;
    
    public Bullet(int x, int y, double angle, boolean circular, boolean direction,boolean speedMod)
    {
        this.x = x;
        this.y = y;
        if(speedMod)
        {
            speed = 5;
        }
        else
            speed = 10;
        this.angle = angle;
        radius = 28;
        c1 = new Color(255, 0, 0 );
        this.circular = circular;
        count = 0;
        this.direction = direction;
        iteration = 0;
        stopChange = false;
    }

    public void move()
    {
        x = x +(int) (speed * Math.cos( angle ) );
        y = y +(int) (speed * Math.sin( angle ) );
        if( stopChange == false)
        {
            count++;
            if(circular && count%3 == 0)
            {
                iteration++;
                if(iteration%7 == 0)
                {
                    stopChange = true;
                }
                count = 0;
                if(direction)
                    angle+=Math.PI/18;
                else
                    angle-=Math.PI/18;
            }
        }
    }
    
    public void draw( Graphics g )
    {
        g.setColor( c1 );
        g.fillOval( x, y, radius, radius );
    }
    
}
