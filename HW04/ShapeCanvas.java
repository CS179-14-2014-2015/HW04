import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
/**
 * Write a description of class Canvas here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class ShapeCanvas extends Canvas
{
    Fighter fight;
    ArrayList<Bullet> array = new ArrayList<Bullet>();
    public ShapeCanvas(int Scr_width, int Scr_height)
    {
        fight = new Fighter( Scr_width/2, Scr_height*3/4, Scr_width-50, Scr_height);
        Color backG = new Color( 255, 255, 255 );
        setBackground( backG );
    }

    public void paint( Graphics g )
    {
        fight.draw(g);
        for(int i = 0; i< array.size(); i++)
        {
            array.get(i).draw(g);
        }
    }
    
    public void addBul( Bullet bullet )
    {
        if( bullet != null)
            array.add(bullet);
    }
    
    public Fighter getFight()
    {
        return fight;
    }
    
    public void moveBul()
    {
        for(int i = 0; i<array.size(); i++)
        {
            array.get(i).move();
            if(array.get(i).x+ array.get(i).radius < 0 || array.get(i).x > 700 ||
               array.get(i).y+ array.get(i).radius < 0 || array.get(i).y > 550)
            {
                array.remove(i);
                i--;//since we removed something then the indexes changed
            }
            
        }
    }
    
    public int checkCollision()
    {
        int num = 0;
        for(int i = 0; i<array.size(); i++)
        {
            if( Math.sqrt( Math.pow( Math.abs( (array.get(i).x+(array.get(i).radius/2))-(fight.x+fight.width/2)), 2) + Math.pow( Math.abs( ( array.get(i).y + (array.get(i).radius/2) )-(fight.y+fight.height/2)), 2) )< (array.get(i).radius + fight.width )/2 )
            {
                num++;
                array.remove(i);
                i--;
            }
        }
        return num;
    }
    
}
