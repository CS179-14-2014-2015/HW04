import java.awt.event.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import javax.imageio.ImageIO;
import java.io.IOException;
import java.util.Random;
import java.util.ArrayList;
/**
 * Write a description of class tester here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class tester
{
    public static void main(String args[])
    {
        BufferedImage img = null;
        int width, height;
        width = 0;
        height = 0;
        ArrayList<Integer> array = new ArrayList<Integer>();
        for(int i = 0; i < 5; i++)
        {
            array.add( i);
        }
        for(int i = 0; i < array.size(); i++)
        {
            System.out.println( array.get( i) );
        }
        for( int i = 0; i < array.size(); i++)
        {
            System.out.println(i +"before removing " + array.get( i) );
            if( array.get(i) == 0 )
            {
                array.remove(i);
            }
            System.out.println(i +"after removing " + array.get( i) );
        }
        for(int i = 0; i < array.size(); i++)
        {
            System.out.println( array.get( i) );
        }
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
        System.out.println( "width: " + width );
        System.out.println( "height: " + height );
    }
}
