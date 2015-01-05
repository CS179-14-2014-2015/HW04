import javax.swing.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;
import java.io.*;
/**
 * Write a description of class GameGui here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class GameGui extends JFrame
{
    ShapeCanvas sc;
    int num = 0;
    JLabel score;
    Fighter fight;
    Bullet[] bul;
    public GameGui(int Scr_width, int Scr_height)
    {
        setLayout( new BorderLayout() );
        sc = new ShapeCanvas( Scr_width, Scr_height);
        score = new JLabel("Score: "+ num);
        add( sc, "Center");
        add( score, "East");
        fight = sc.getFight();
        this.addKeyListener(new keyBoard());
        sc.addKeyListener(new keyBoard());
    }
    private class keyBoard implements KeyListener
    {
            int key;
            int vert;
            int hori;
            public void keyPressed(KeyEvent e) 
            {
                key = e.getKeyCode();
                vert = 0;
                hori = 0;
                if (key == KeyEvent.VK_LEFT) 
                {
                    hori = -1;
                }
                else
                if (key == KeyEvent.VK_RIGHT) 
                {
                    hori = 1;
                }

                if (key == KeyEvent.VK_UP) 
                {
                    vert = -1;
                }
                else
                if (key == KeyEvent.VK_DOWN) 
                {
                    vert = 1;
                }
                fight.move(hori, vert);
                sc.repaint();
            }
            
            public void keyTyped(KeyEvent e)
            {
                key = e.getKeyCode();
                vert = 0;
                hori = 0;
                if (key == KeyEvent.VK_LEFT) 
                {
                    hori = -1;
                }
                else
                if (key == KeyEvent.VK_RIGHT) 
                {
                    hori = 1;
                }

                if (key == KeyEvent.VK_UP) 
                {
                    vert = -1;
                }
                else
                if (key == KeyEvent.VK_DOWN) 
                {
                    vert = 1;
                }
                fight.move(hori, vert);
                sc.repaint();
            }
            
            public void keyReleased(KeyEvent e) 
            {
                
            }
            
    }
    
    public void addBul(Bullet bullet)
    {
        sc.addBul(bullet);
        sc.repaint();
    }
    
    public void moveBul()
    {
        sc.moveBul();
        sc.repaint();
    }
    
    public void updateScore()
    {
        num += sc.checkCollision();
        score.setText("Score:" + num);
    }
    
}
