import java.awt.event.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.io.IOException;
/**
 * Write a description of class Bullet here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class Fighter 
{
    int x, y, speed, width, height, scr_width, scr_height;
    double angle;
    BufferedImage img = null;
    public Fighter(int x, int y, int scr_width, int scr_height)
    {
        this.x = x;
        this.y = y;
        speed = 10;
        this.angle = angle;
        this.scr_width = scr_width;
        this.scr_height = scr_height;
        try 
        {
            img = ImageIO.read(new File("fighter.bmp"));
        } catch (IOException e) 
        {
            
        }
        if(img != null)
        {
            width = img.getWidth();
            height = img.getHeight();
        }
    }

    public void move(int velx, int vely)
    {
        x+= speed*velx;
        if( x <0 || ( x + width )> scr_width )
        {
            x-=speed*velx;
        }
        y+= speed*vely;
        if( y <0 || ( y + height )> scr_height-20 )
        {
            y-=speed*vely;
        }
       
    }
    
    public void draw( Graphics g )
    {
        g.drawImage( img, x, y, null );
    }
    
}
